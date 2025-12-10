#pragma once
#include "visitor.h"
#include <memory>

class BattleVisitor : public Visitor {
public:
    explicit BattleVisitor(double range);
    virtual void visit(Dungeon& d) override;
private:
    double range_;
};
