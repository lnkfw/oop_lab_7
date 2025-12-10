#pragma once
#include <string>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onDeath(const std::string& victim, const std::string& killer) = 0;
};
