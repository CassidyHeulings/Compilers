#include <string>
#include "Logger.hpp"
#pragma once

// class to hold parser information and functions
class Parser {
public:
    Parser(Logger& loggerInstance, const std::string& processName);
private:
    // logger instance
    Logger& logger;
    // name of this process for the logger
    std::string name;
};