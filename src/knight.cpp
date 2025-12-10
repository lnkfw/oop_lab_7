#include "knight.h"
#include "visitor.h"

Knight::Knight(const std::string& name, double x, double y)
    : NPC("Knight", name, x, y) {}

void Knight::accept(Visitor& v) {
}
