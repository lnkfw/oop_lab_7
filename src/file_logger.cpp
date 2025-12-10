#include "file_logger.h"
#include <fstream>

FileLogger::FileLogger(const std::string& filename) : filename_(filename) {}

void FileLogger::onDeath(const std::string& victim, const std::string& killer) {
    std::ofstream ofs(filename_, std::ios::app);
    if (!ofs) return;
    if (killer.empty()) {
        ofs << "[LOG] " << victim << " died.\n";
    } else {
        ofs << "[LOG] " << victim << " was killed by " << killer << ".\n";
    }
}
