#include "../Headers/ErrorHandler.hpp"

ErrorHandler::ErrorHandler() {
    errorCounter = 0;
    warningCounter = 0;
};

std::string ErrorHandler::getErrorMessage(int code) {
    errorCounter++;
    return eflags[code];
}

std::string ErrorHandler::getWarningMessage(int code) {
    warningCounter++;
    return wflags[code];
}

int ErrorHandler::getNumErrors() {
    return errorCounter;
}

int ErrorHandler::getNumWarnings() {
    return warningCounter;
}