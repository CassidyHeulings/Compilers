#include "../Headers/ErrorHandler.hpp"

ErrorHandler::ErrorHandler(){};

std::string ErrorHandler::getErrorMessage(int code) {
    return flags[code];
}