#pragma once
#include "npc.h"
#include "observer.h"
#include <vector>
#include <memory>
#include <string>

class Visitor;
class NPCFactory;

class Dungeon {
public:
    Dungeon();

    bool addNPC(NPCPtr npc);
    bool removeByName(const std::string& name);
    NPCPtr findByName(const std::string& name) const;

    const std::vector<NPCPtr>& getNPCs() const;

    void saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename, NPCFactory& factory);

    void accept(Visitor& v);

    void addObserver(std::shared_ptr<Observer> obs);
    void removeObserver(std::shared_ptr<Observer> obs);

    void notifyDeath(const std::string& victim, const std::string& killer);

    void removeNames(const std::vector<std::string>& names);

private:
    std::vector<NPCPtr> npcs_;
    std::vector<std::shared_ptr<Observer>> observers_;
};
