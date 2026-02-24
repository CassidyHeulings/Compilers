#include "../Headers/Logger.hpp"
#include <iostream>

Logger::Logger(bool isDebugOn) {
    debugOn = isDebugOn;
}

void Logger::debug(const std::string& statement) {
    if (debugOn)
        std::cout << "[DEBUG] " << statement << std::endl;
}

void Logger::info(const std::string& statement) {
    std::cout << "[INFO] " << statement << std::endl;
}