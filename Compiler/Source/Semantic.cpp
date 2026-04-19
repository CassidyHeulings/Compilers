#include "../Headers/Semantic.hpp"

Semantic::Semantic(Logger& loggerInstance, const std::string& processName) 
    : logger(loggerInstance) {
    name = processName;
    logger.test(name, "hi im semantic analysis");
}