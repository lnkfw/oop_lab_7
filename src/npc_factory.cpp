#include "npc_factory.h"
#include "dragon.h"
#include "frog.h"
#include "knight.h"
#include <algorithm>

class SimpleNPCFactory : public NPCFactory {
public:
    NPCPtr create(const std::string& type,
                  const std::string& name,
                  double x, double y) override {
        std::string t = type;
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);
        if (t == "dragon") return std::make_shared<Dragon>(name, x, y);
        if (t == "frog" || t == "жаба") return std::make_shared<Frog>(name, x, y);
        if (t == "knight" || t == "рыцарь") return std::make_shared<Knight>(name, x, y);
        return nullptr;
    }
};
std::unique_ptr<NPCFactory> make_default_factory() {
    return std::make_unique<SimpleNPCFactory>();
}
