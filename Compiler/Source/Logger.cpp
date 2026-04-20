#include "../Headers/Logger.hpp"
#include <iostream>

Logger::Logger(ErrorHandler& errorHandlerInstance, bool isDebugOn, bool isTestingOn) : errorHandler(errorHandlerInstance) {
    debugOn = isDebugOn;
    testingOn = isTestingOn;
}

void Logger::debug(const std::string& currProcess, const std::string& statement) {
    if (debugOn)
        std::cout << "  \033[32m[DEBUG]\033[0m " << currProcess << " - " << statement << std::endl;
}

void Logger::info(const std::string& currProcess, const std::string& statement) {
    std::cout << "   \033[36m[INFO]\033[0m " << currProcess << " - " << statement << std::endl;
}

void Logger::test(const std::string& currProcess, const std::string& statement) {
    if (testingOn)
        std::cout << "   \033[34m[TEST]\033[0m " << currProcess << " - " << statement << std::endl;
}

void Logger::error(const std::string& currProcess, int errorCode, const std::string& errorCause) {
    std::string errorMessage = errorHandler.getErrorMessage(errorCode);
    std::cout << "  \033[31m[ERROR] " << currProcess << " - " << errorMessage << " -> " << errorCause << "\033[0m" << std::endl;
}

void Logger::warning(const std::string& currProcess, int warningCode, const std::string& warningFix) {
    std::string warningMessage = errorHandler.getWarningMessage(warningCode);
    std::cout << "\033[33m[WARNING] " << currProcess << " - " << warningMessage << " -> " << warningFix << "\033[0m" << std::endl;
}

void Logger::startProcess(const std::string& currProcess) {
    std::cout << "\n\033[34m******************** \033[35m" << currProcess << "\033[34m ********************\033[0m" << std::endl;
};

void Logger::startProgram(int progNum) {
    std::cout << "\n\033[36m============================================================" << std::endl;
	std::cout << "======================== \033[35mProgram #" + std::to_string(progNum) + "\033[36m ========================" << std::endl;
	std::cout << "============================================================\033[0m" << std::endl;
}

bool Logger::endProcess(const std::string& currProcess) {
    int numErrors = errorHandler.getNumErrors();
    int numWarnings = errorHandler.getNumWarnings();
    info(currProcess, "Finished. ");
    std::cout << "~~~~~~ \033[35m" + currProcess + "\033[0m has \033[31m" << numErrors << " errors\033[0m and \033[33m" << numWarnings << " warnings\033[0m ~~~~~~" << std::endl;
    return (numErrors > 0);
};

void Logger::endProgram() {
    int numErrors = errorHandler.getNumErrors();
    int numWarnings = errorHandler.getNumWarnings();
    std::cout << "\n~~~~~~ \033[35mPrograms\033[0m have \033[31m" << numErrors << " errors\033[0m and \033[33m" << numWarnings << " warnings\033[0m ~~~~~~" << std::endl;
}

bool Logger::getDebug() {
    return debugOn;
}