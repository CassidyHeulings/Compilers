#include "../Headers/Logger.hpp"
#include <iostream>

Logger::Logger(bool isDebugOn) {
    debugOn = isDebugOn;
}

void Logger::debug(std::string currProcess, const std::string& statement) {
    if (debugOn)
        std::cout << "[DEBUG] " << currProcess << " - " << statement << std::endl;
}

void Logger::info(std::string currProcess, const std::string& statement) {
    std::cout << "[INFO] " << currProcess << " - " << statement << std::endl;
}