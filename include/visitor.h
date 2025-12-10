#pragma once
class Dungeon;
class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(Dungeon& d) = 0;
};
