#pragma once
#include <string>
#include "npc.h"

class NPCFactory {
public:
    virtual ~NPCFactory() = default;
    virtual NPCPtr create(const std::string& type,
                          const std::string& name,
                          double x, double y) = 0;
};
