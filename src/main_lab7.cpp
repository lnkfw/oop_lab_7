#include <iostream>
#include <memory>
#include "dungeon.h"
#include "npc_factory.h"
#include "game.h"
#include "console_logger.h"
#include "file_logger.h"

std::unique_ptr<NPCFactory> make_default_factory();

int main() {
    Dungeon d;
    auto factory = make_default_factory();
    d.addObserver(std::make_shared<ConsoleLogger>());
    d.addObserver(std::make_shared<FileLogger>("log.txt"));

    Game g(d, std::move(factory), 300, 300);
    g.init_random(50);

    std::cout << "Начало симуляции\n";
    g.run_for(std::chrono::seconds(30));
    std::cout << "Симуляция завершена\n";
    return 0;
}
