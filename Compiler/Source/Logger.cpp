#include "../Headers/Logger.hpp"
#include <iostream>

Logger::Logger(ErrorHandler& errorHandlerInstance, bool isDebugOn, bool isTestingOn) : errorHandler(errorHandlerInstance) {
    debugOn = isDebugOn;
    testingOn = isTestingOn;
}

void Logger::debug(const std::string& currProcess, const std::string& statement) {
    if (debugOn)
        std::cout << "\033[32m[DEBUG]\033[0m " << currProcess << " - " << statement << std::endl;
}

void Logger::info(const std::string& currProcess, const std::string& statement) {
    std::cout << " \033[36m[INFO]\033[0m " << currProcess << " - " << statement << std::endl;
}

void Logger::test(const std::string& currProcess, const std::string& statement) {
    if (testingOn)
        std::cout << " \033[34m[TEST]\033[0m " << currProcess << " - " << statement << std::endl;
}

void Logger::error(const std::string& currProcess, int errorCode, const std::string& errorCause) {
    std::string errorMessage = errorHandler.getErrorMessage(errorCode);
    std::cout << "\033[31m[ERROR] " << currProcess << " - " << errorMessage << " -> " << errorCause << "\033[0m" << std::endl;
}

void Logger::warning(const std::string& currProcess, int warningCode, const std::string& warningCause) {
    std::string warningMessage = errorHandler.getWarningMessage(warningCode);
    std::cout << "\033[33m[WARNING] " << currProcess << " - " << warningMessage << " -> " << warningCause << "\033[0m" << std::endl;
}

void Logger::startProcess(const std::string& currProcess) {
    std::cout << " ============== \033[35m" << currProcess << "\033[0m ==============" << std::endl;
};

bool Logger::endProcess(const std::string& currProcess) {
    info(currProcess, " Finished. ");
    //std::cout << " ========= " << currProcess << " is Finished =========\n" << std::endl;
    int numErrors = errorHandler.getNumErrors();
    return (numErrors > 0);
};

void Logger::endProgram(const std::string& currProcess) {
    int numErrors = errorHandler.getNumErrors();
    int numWarnings = errorHandler.getNumWarnings();
    //std::cout << " ==========  \033[35mProgram is Finished\033[0m  ==========" << std::endl;
    std::cout << "~~~~~~ \033[35m" << currProcess << "\033[0m has \033[31m" << numErrors << " errors\033[0m and \033[33m" << numWarnings << " warnings\033[0m ~~~~~~" << std::endl;
}