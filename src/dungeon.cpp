#include "dungeon.h"
#include "npc_factory.h"
#include "visitor.h"
#include "file_npc_factory.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

Dungeon::Dungeon() {}

bool Dungeon::addNPC(NPCPtr npc) {
    if (!npc) return false;
    if (npc->getX() < 0 || npc->getX() > 500 || npc->getY() < 0 || npc->getY() > 500) return false;
    for (auto& n : npcs_) {
        if (n->getName() == npc->getName()) return false;
    }
    npcs_.push_back(npc);
    return true;
}

bool Dungeon::removeByName(const std::string& name) {
    auto it = std::remove_if(npcs_.begin(), npcs_.end(), [&](const NPCPtr& n){ return n->getName() == name; });
    if (it == npcs_.end()) return false;
    npcs_.erase(it, npcs_.end());
    return true;
}

NPCPtr Dungeon::findByName(const std::string& name) const {
    for (auto& n : npcs_) if (n->getName() == name) return n;
    return nullptr;
}

const std::vector<NPCPtr>& Dungeon::getNPCs() const { return npcs_; }

void Dungeon::saveToFile(const std::string& filename) const {
    std::ofstream ofs(filename);
    for (auto& n : npcs_) {
        ofs << n->getType() << " " << n->getName() << " " << n->getX() << " " << n->getY() << "\n";
    }
}

bool Dungeon::loadFromFile(const std::string& filename, NPCFactory& factory) {
    std::ifstream ifs(filename);
    if (!ifs) return false;
    std::string line;
    std::vector<NPCPtr> loaded;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;
        NPCPtr p = FileNPCFactory::createFromLine(line, factory);
        if (!p) continue;
        bool duplicate = false;
        for (auto& ex : npcs_) if (ex->getName() == p->getName()) duplicate = true;
        if (!duplicate) loaded.push_back(p);
    }
    for (auto& p : loaded) npcs_.push_back(p);
    return true;
}

void Dungeon::accept(Visitor& v) {
    v.visit(*this);
}

void Dungeon::addObserver(std::shared_ptr<Observer> obs) {
    observers_.push_back(obs);
}

void Dungeon::removeObserver(std::shared_ptr<Observer> obs) {
    observers_.erase(std::remove(observers_.begin(), observers_.end(), obs), observers_.end());
}

void Dungeon::notifyDeath(const std::string& victim, const std::string& killer) {
    for (auto& o : observers_) {
        o->onDeath(victim, killer);
    }
}

void Dungeon::removeNames(const std::vector<std::string>& names) {
    for (auto& nm : names) {
        removeByName(nm);
    }
}
