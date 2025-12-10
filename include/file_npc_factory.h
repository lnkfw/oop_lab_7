#pragma once
#include "npc_factory.h"
#include <string>
#include <istream>

class FileNPCFactory {
public:
    static NPCPtr createFromLine(const std::string& line, NPCFactory& baseFactory);
};
