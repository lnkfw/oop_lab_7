#include <iostream>
#include <memory>
#include <string>
#include "dungeon.h"
#include "npc_factory.h"
#include "file_npc_factory.h"
#include "battle_visitor.h"
#include "console_logger.h"
#include "file_logger.h"

std::unique_ptr<NPCFactory> make_default_factory();

int main() {
    Dungeon d;
    auto factory = make_default_factory();
    d.addObserver(std::make_shared<ConsoleLogger>());
    d.addObserver(std::make_shared<FileLogger>("log.txt"));

    while (true) {
        std::cout << "Menu:\n";
        std::cout << "1) Add NPC\n2) Save\n3) Load\n4) List\n5) Battle\n6) Remove by name\n7) Exit\nChoose: ";
        int cmd;
        if (!(std::cin >> cmd)) break;
        if (cmd == 1) {
            std::string type, name;
            double x, y;
            std::cout << "Enter type (Dragon/Frog/Knight): ";
            std::cin >> type;
            std::cout << "Enter unique name: ";
            std::cin >> name;
            std::cout << "Enter x y (0..500): ";
            std::cin >> x >> y;
            NPCPtr p = factory->create(type, name, x, y);
            if (!p) { std::cout << "Unknown type\n"; continue; }
            if (!d.addNPC(p)) { std::cout << "Failed to add (duplicate name or out of bounds)\n"; continue; }
            std::cout << "Added\n";
        } else if (cmd == 2) {
            std::string fn;
            std::cout << "Filename: "; std::cin >> fn;
            d.saveToFile(fn);
            std::cout << "Saved\n";
        } else if (cmd == 3) {
            std::string fn;
            std::cout << "Filename: "; std::cin >> fn;
            if (d.loadFromFile(fn, *factory)) std::cout << "Loaded\n"; else std::cout << "Load failed\n";
        } else if (cmd == 4) {
            std::cout << "NPC list:\n";
            for (auto& n : d.getNPCs()) {
                std::cout << n->getType() << " " << n->getName() << " (" << n->getX() << ", " << n->getY() << ")\n";
            }
        } else if (cmd == 5) {
            double range;
            std::cout << "Enter battle range: ";
            std::cin >> range;
            BattleVisitor bv(range);
            d.accept(bv);
            std::cout << "Battle round completed\n";
        } else if (cmd == 6) {
            std::string name;
            std::cout << "Name: "; std::cin >> name;
            if (d.removeByName(name)) std::cout << "Removed\n"; else std::cout << "Not found\n";
        } else if (cmd == 7) {
            break;
        } else {
            std::cout << "Unknown\n";
        }
    }

    return 0;
}
