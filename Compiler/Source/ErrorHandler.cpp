#include "../Headers/ErrorHandler.hpp"

ErrorHandler::ErrorHandler() {
    errorCounter = 0;
    warningCounter = 0;
    allErrorCounter = 0;
    allWarningCounter = 0;
};

std::string ErrorHandler::getErrorMessage(int code) {
    errorCounter++;
    allErrorCounter++;
    return eflags[code];
}

std::string ErrorHandler::getWarningMessage(int code) {
    warningCounter++;
    allWarningCounter++;
    return wflags[code];
}

void ErrorHandler::resetCounters() {
    errorCounter = 0;
    warningCounter = 0;
}

int ErrorHandler::getNumErrors() {
    return errorCounter;
}

int ErrorHandler::getNumWarnings() {
    return warningCounter;
}

int ErrorHandler::getNumAllErrors() {
    return allErrorCounter;
}

int ErrorHandler::getNumAllWarnings() {
    return allWarningCounter;
}