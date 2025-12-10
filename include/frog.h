#pragma once
#include "npc.h"

class Frog : public NPC {
public:
    Frog(const std::string& name, double x, double y);
    virtual void accept(Visitor& v) override;
};
