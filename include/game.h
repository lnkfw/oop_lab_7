#pragma once
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include <shared_mutex>
#include <random>
#include <chrono>
#include <vector>

class Dungeon;
class NPCFactory;
using NPCPtr = std::shared_ptr<class NPC>;

struct FightEvent {
    NPCPtr attacker;
    NPCPtr defender;
};

class Game {
public:
    Game(Dungeon& dungeon, std::unique_ptr<NPCFactory> factory, int maxX = 100, int maxY = 100);
    ~Game();

    void init_random(size_t count = 50);

    void run_for(std::chrono::seconds duration);

    void stop();

private:
    void movement_loop();
    void fight_loop();
    void print_map_once();

    void enqueue_nearby();

    Dungeon& dungeon_;
    std::unique_ptr<NPCFactory> factory_;

    std::vector<FightEvent> events_;
    std::mutex events_mtx_;

    std::thread move_thread_;
    std::thread fight_thread_;
    std::mutex cout_mtx_;

    mutable std::shared_mutex dungeon_mtx_;

    std::atomic<bool> running_;
    int max_x_, max_y_;

    std::mt19937 rng_;
};
