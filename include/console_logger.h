#pragma once
#include "observer.h"
#include <string>

class ConsoleLogger : public Observer {
public:
    virtual void onDeath(const std::string& victim, const std::string& killer) override;
};
