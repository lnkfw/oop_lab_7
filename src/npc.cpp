#include "npc.h"
#include "visitor.h"
#include <algorithm>
#include <cmath>

NPC::NPC(std::string type, std::string name, double x, double y)
    : type_(std::move(type)), name_(std::move(name)), x_(x), y_(y), alive_(true) {}

std::string NPC::getType() const { return type_; }
std::string NPC::getName() const { return name_; }

double NPC::getX() const {
    std::lock_guard<std::mutex> lg(mtx_);
    return x_;
}
double NPC::getY() const {
    std::lock_guard<std::mutex> lg(mtx_);
    return y_;
}

void NPC::setPosition(double nx, double ny) {
    std::lock_guard<std::mutex> lg(mtx_);
    x_ = nx; y_ = ny;
}

void NPC::move(double dx, double dy, int maxX, int maxY) {
    std::lock_guard<std::mutex> lg(mtx_);
    if (!alive_) return;
    double nx = x_ + dx;
    double ny = y_ + dy;
    if (nx < 0) nx = 0;
    if (ny < 0) ny = 0;
    if (nx > maxX) nx = maxX;
    if (ny > maxY) ny = maxY;
    x_ = nx; y_ = ny;
}

bool NPC::is_close(const std::shared_ptr<NPC>& other, double distance) const {
    if (!other) return false;
    // lock both to read positions (avoid deadlock by locking in address order)
    const NPC* a = this;
    const NPC* b = other.get();
    if (a == b) return false;
    std::unique_lock<std::mutex> la(mtx_, std::defer_lock);
    std::unique_lock<std::mutex> lb(b->mtx_, std::defer_lock);
    std::lock(la, lb);
    double dx = x_ - b->x_;
    double dy = y_ - b->y_;
    double d = std::sqrt(dx*dx + dy*dy);
    return d <= distance;
}

bool NPC::is_alive() const {
    std::lock_guard<std::mutex> lg(mtx_);
    return alive_;
}

void NPC::must_die() {
    std::lock_guard<std::mutex> lg(mtx_);
    alive_ = false;
}

void NPC::accept(Visitor& v) {
    (void)v;
}
