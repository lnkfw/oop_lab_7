#pragma once
#include "npc.h"

class Knight : public NPC {
public:
    Knight(const std::string& name, double x, double y);
    virtual void accept(Visitor& v) override;
};
