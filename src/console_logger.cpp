#include "console_logger.h"
#include <iostream>

void ConsoleLogger::onDeath(const std::string& victim, const std::string& killer) {
    if (killer.empty()) {
        std::cout << "[LOG] " << victim << " died.\n";
    } else {
        std::cout << "[LOG] " << victim << " was killed by " << killer << ".\n";
    }
}
