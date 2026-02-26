#include "../Headers/Logger.hpp"
#include <iostream>

Logger::Logger(ErrorHandler& errorHandlerInstance, bool isDebugOn, bool isTestingOn) : errorHandler(errorHandlerInstance) {
    debugOn = isDebugOn;
    testingOn = isTestingOn;
}

void Logger::debug(const std::string& currProcess, const std::string& statement) {
    if (debugOn)
        std::cout << "[DEBUG] " << currProcess << " - " << statement << std::endl;
}

void Logger::info(const std::string& currProcess, const std::string& statement) {
    std::cout << " [INFO] " << currProcess << " - " << statement << std::endl;
}

void Logger::test(const std::string& currProcess, const std::string& statement) {
    if (testingOn)
        std::cout << " [TEST] " << currProcess << " - " << statement << std::endl;
}

void Logger::error(const std::string& currProcess, int errorCode, const std::string& errorCause) {
    std::string errorMessage = errorHandler.getErrorMessage(errorCode);
    std::cout << "[ERROR] " << currProcess << " - " << errorMessage << " -> " << errorCause << std::endl;
}

bool Logger::endProcess(const std::string& currProcess) {
    std::cout << " ========= " << currProcess << " is Finished =========\n" << std::endl;
    int numErrors = errorHandler.getNumErrors();
    return (numErrors > 0);
};

void Logger::endProgram(const std::string& currProcess) {
    int numErrors = errorHandler.getNumErrors();
    int numWarnings = errorHandler.getNumWarnings();
    std::cout << "~~~~~~ " << currProcess << " has " << numErrors << " errors and " << numWarnings << "warnings ~~~~~~" << std::endl;
    std::cout << " ==========  Program is Finished  ==========" << std::endl;
}