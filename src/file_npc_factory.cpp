#include "file_npc_factory.h"
#include "npc_factory.h"
#include <sstream>
#include <algorithm>
#include <cctype>

NPCPtr FileNPCFactory::createFromLine(const std::string& line, NPCFactory& baseFactory) {
    std::istringstream iss(line);
    std::string type;
    std::string name;
    double x, y;
    if (!(iss >> type >> name >> x >> y)) {
        return nullptr;
    }
    return baseFactory.create(type, name, x, y);
}
