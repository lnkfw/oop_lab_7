#pragma once
#include <string>
#include <memory>
#include <mutex>

class Visitor;
class Dungeon;

class NPC {
public:
    NPC(std::string type, std::string name, double x, double y);
    virtual ~NPC() = default;

    std::string getType() const;
    std::string getName() const;
    double getX() const;
    double getY() const;

    void setPosition(double nx, double ny);
    virtual void move(double dx, double dy, int maxX, int maxY);
    bool is_close(const std::shared_ptr<NPC>& other, double distance) const;
    bool is_alive() const;
    void must_die();

    virtual void accept(Visitor& v);

private:
    std::string type_;
    std::string name_;
    double x_;
    double y_;
    bool alive_;
    mutable std::mutex mtx_;
};
using NPCPtr = std::shared_ptr<NPC>;
