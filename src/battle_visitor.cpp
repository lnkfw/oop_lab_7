#include "battle_visitor.h"
#include "dungeon.h"
#include "npc.h"
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

static double dist(const NPCPtr& a, const NPCPtr& b) {
    double dx = a->getX() - b->getX();
    double dy = a->getY() - b->getY();
    return std::sqrt(dx*dx + dy*dy);
}

BattleVisitor::BattleVisitor(double range) : range_(range) {}

void BattleVisitor::visit(Dungeon& d) {
    const auto& npcs = d.getNPCs();
    std::vector<std::string> toRemove;

    // pairwise battles
    for (size_t i = 0; i < npcs.size(); ++i) {
        for (size_t j = i + 1; j < npcs.size(); ++j) {
            auto a = npcs[i];
            auto b = npcs[j];
            if (!a || !b) continue;
            if (std::find(toRemove.begin(), toRemove.end(), a->getName()) != toRemove.end()) continue;
            if (std::find(toRemove.begin(), toRemove.end(), b->getName()) != toRemove.end()) continue;

            if (dist(a, b) <= range_) {
                std::string at = a->getType();
                std::string bt = b->getType();

                bool aDies = false;
                bool bDies = false;
                std::string killerOfA = "environment";
                std::string killerOfB = "environment";

                if (at == "Frog") {
                    bDies = true;
                    killerOfB = a->getName();
                }
                if (bt == "Frog") {
                    aDies = true;
                    killerOfA = b->getName();
                }
                if (!aDies && !bDies) {
                    if (at == "Knight" && bt == "Dragon") {
                        bDies = true;
                        killerOfB = a->getName();
                    } else if (at == "Dragon" && bt == "Knight") {
                        aDies = true;
                        killerOfA = b->getName();
                    }
                }
                // две жабы = съедают друг друга
                if (at == "Frog" && bt == "Frog") {
                    aDies = true;
                    bDies = true;
                    killerOfA = b->getName();
                    killerOfB = a->getName();
                }

                if (aDies) {
                    toRemove.push_back(a->getName());
                    d.notifyDeath(a->getName(), killerOfA);
                }
                if (bDies) {
                    toRemove.push_back(b->getName());
                    d.notifyDeath(b->getName(), killerOfB);
                }
            }
        }
    }

    if (!toRemove.empty()) {
        d.removeNames(toRemove);
    }
}
