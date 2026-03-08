#include "../Headers/Parser.hpp"
#include <iostream>

Parser::Parser(Logger& loggerInstance, const std::string& processName) : logger(loggerInstance) {
    name = processName;
    logger.test(name, "hi im the parser");
}