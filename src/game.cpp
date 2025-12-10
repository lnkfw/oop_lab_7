#include "game.h"
#include "dungeon.h"
#include "npc_factory.h"
#include "npc.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <random>

using namespace std::chrono_literals;

struct TypeParams { int move; int kill; char symbol; };

static TypeParams params_for(const std::string &t) {
    if (t == "Dragon") return {50, 30, 'D'};
    if (t == "Frog"  || t == "Жаба") return {1, 10, 'F'};
    if (t == "Knight" || t == "Рыцарь" || t == "рыцарь") return {30, 10, 'K'};
    return {5,5,'?'};
}

Game::Game(Dungeon& dungeon, std::unique_ptr<NPCFactory> factory, int maxX, int maxY)
    : dungeon_(dungeon), factory_(std::move(factory)), running_(false),
      max_x_(maxX), max_y_(maxY)
{
    std::random_device rd;
    rng_.seed(rd());
}

Game::~Game() {
    stop();
}

void Game::init_random(size_t count) {
    std::uniform_int_distribution<int> dx(0, max_x_);
    std::uniform_int_distribution<int> dy(0, max_y_);
    std::vector<std::string> types = {"Dragon", "Frog", "Knight"};

    for (size_t i = 0; i < count; ++i) {
        std::string type = types[rng_() % types.size()];
        std::string name = type + std::to_string(i + 1);
        double x = dx(rng_);
        double y = dy(rng_);
        auto npc = factory_->create(type, name, x, y);
        if (npc) {
            std::unique_lock<std::shared_mutex> ul(dungeon_mtx_);
            dungeon_.addNPC(npc);
        }
    }
}

void Game::run_for(std::chrono::seconds duration) {
    running_ = true;

    fight_thread_ = std::thread([this]{ this->fight_loop(); });

    move_thread_ = std::thread([this]{ this->movement_loop(); });

    auto start = std::chrono::steady_clock::now();
    while (running_) {
        print_map_once();
        std::this_thread::sleep_for(1s);

        if (std::chrono::steady_clock::now() - start >= duration) {
            running_ = false;
            break;
        }
    }

    if (move_thread_.joinable()) move_thread_.join();
    if (fight_thread_.joinable()) fight_thread_.join();

    {
        std::shared_lock<std::shared_mutex> sl(dungeon_mtx_);
        std::cout << "\nВыжившие:\n";
        for (auto &n : dungeon_.getNPCs()) {
            if (n && n->is_alive()) {
                std::cout << n->getType() << " " << n->getName()
                          << " (" << n->getX() << ", " << n->getY() << ")\n";
            }
        }
    }
}

void Game::stop() {
    running_ = false;
}

void Game::movement_loop() {
    std::uniform_int_distribution<int> small(-1,1);
    while (running_) {
        {
            std::shared_lock<std::shared_mutex> sl(dungeon_mtx_);
            for (auto &n : dungeon_.getNPCs()) {
                if (!n) continue;
                if (!n->is_alive()) continue;
                auto tp = params_for(n->getType());
                std::uniform_int_distribution<int> step(-tp.move, tp.move);
                int dx = step(rng_) / 10;
                int dy = step(rng_) / 10;
                n->move(dx, dy, max_x_, max_y_);
            }
        }

        enqueue_nearby();
        std::this_thread::sleep_for(10ms);
    }
}

void Game::enqueue_nearby() {
    std::shared_lock<std::shared_mutex> sl(dungeon_mtx_);
    const auto &list = dungeon_.getNPCs();
    std::vector<FightEvent> new_events;
    for (size_t i = 0; i < list.size(); ++i) {
        auto a = list[i];
        if (!a || !a->is_alive()) continue;
        for (size_t j = i + 1; j < list.size(); ++j) {
            auto b = list[j];
            if (!b || !b->is_alive()) continue;
            int ka = params_for(a->getType()).kill;
            int kb = params_for(b->getType()).kill;
            if (a->is_close(b, ka)) new_events.push_back({a,b});
            if (b->is_close(a, kb)) new_events.push_back({b,a});
        }
    }
    if (!new_events.empty()) {
        std::lock_guard<std::mutex> lg(events_mtx_);
        for (auto &e : new_events) events_.push_back(std::move(e));
    }
}

void Game::fight_loop() {
    std::uniform_int_distribution<int> die(1,6);
    std::mt19937 rng_local(std::random_device{}());

    while (running_) {
        FightEvent ev;
        bool have = false;
        {
            std::lock_guard<std::mutex> lg(events_mtx_);
            if (!events_.empty()) {
                ev = std::move(events_.back());
                events_.pop_back();
                have = true;
            }
        }

        if (!have) { std::this_thread::sleep_for(20ms); continue; }

        auto a = ev.attacker;
        auto d = ev.defender;
        if (!a || !d) continue;
        if (!a->is_alive() || !d->is_alive()) continue;

        int attack = die(rng_local);
        int defend = die(rng_local);

        if (attack > defend) {
            {
                std::unique_lock<std::shared_mutex> ul(dungeon_mtx_);
                d->must_die();
                dungeon_.notifyDeath(d->getName(), a->getName());
            }
        }
    }
}

void Game::print_map_once() {
    const int grid = 20;
    std::vector<char> fields(grid * grid, 0);
    int step_x = std::max(1, max_x_ / grid);
    int step_y = std::max(1, max_y_ / grid);

    {
        std::shared_lock<std::shared_mutex> sl(dungeon_mtx_);
        for (auto &npc : dungeon_.getNPCs()) {
            if (!npc) continue;
            if (!npc->is_alive()) continue;
            int x = static_cast<int>(npc->getX());
            int y = static_cast<int>(npc->getY());
            int i = std::clamp(x / step_x, 0, grid - 1);
            int j = std::clamp(y / step_y, 0, grid - 1);
            fields[i + j * grid] = params_for(npc->getType()).symbol;
        }
    }

    std::lock_guard<std::mutex> lg(cout_mtx_);
    for (int j = 0; j < grid; ++j) {
        for (int i = 0; i < grid; ++i) {
            char c = fields[i + j * grid];
            if (c != 0) std::cout << "[" << c << "]";
            else std::cout << "[ ]";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}
