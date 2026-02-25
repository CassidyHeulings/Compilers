#include "../Headers/Logger.hpp"
#include <iostream>

Logger::Logger(ErrorHandler& errorHandlerInstance, bool isDebugOn) : errorHandler(errorHandlerInstance) {
    debugOn = isDebugOn;
}

void Logger::debug(std::string currProcess, const std::string& statement) {
    if (debugOn)
        std::cout << "[DEBUG] " << currProcess << " - " << statement << std::endl;
}

void Logger::info(std::string currProcess, const std::string& statement) {
    std::cout << "[INFO] " << currProcess << " - " << statement << std::endl;
}

void Logger::error(std::string currProcess, int errorCode, std::string& errorCause) {
    std::string errorMessage = errorHandler.getErrorMessage(errorCode);
    std::cout << "[ERROR] " << currProcess << " - " << errorMessage << " -> " << errorCause << std::endl;
}