#pragma once
#include "npc.h"

class Dragon : public NPC {
public:
    Dragon(const std::string& name, double x, double y);
    virtual void accept(Visitor& v) override;
};
