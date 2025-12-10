#include "frog.h"
#include "visitor.h"

Frog::Frog(const std::string& name, double x, double y)
    : NPC("Frog", name, x, y) {}

void Frog::accept(Visitor& v) {
}
