#pragma once
#include "observer.h"
#include <string>

class FileLogger : public Observer {
public:
    FileLogger(const std::string& filename);
    virtual void onDeath(const std::string& victim, const std::string& killer) override;
private:
    std::string filename_;
};
