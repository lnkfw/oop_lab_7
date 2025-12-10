#include "dragon.h"
#include "visitor.h"

Dragon::Dragon(const std::string& name, double x, double y)
    : NPC("Dragon", name, x, y) {}

void Dragon::accept(Visitor& v) {
}
