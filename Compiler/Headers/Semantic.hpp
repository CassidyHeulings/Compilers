#include "Logger.hpp"
#pragma once

// class for semantic analysis creating the AST
class Semantic {
public:
    Semantic(Logger& loggerInstance, const std::string& processName);
private:
    // logger instance
    Logger& logger;
    // name of the process for the logger
    std::string name;
};