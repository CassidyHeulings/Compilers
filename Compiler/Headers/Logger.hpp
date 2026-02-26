#include <string>
#include "ErrorHandler.hpp"
#pragma once

class Logger {
private:
    ErrorHandler& errorHandler;
    bool debugOn;
    bool testingOn;
public:
    Logger(ErrorHandler& errorHandlerInstance, bool isDebugOn, bool isTestingOn);
    // print debug, testing, and info messages from main compiler
    void debug(const std::string& currProcess, const std::string& statement);
    void info(const std::string& currProcess, const std::string& statement);
    void test(const std::string& currProcess, const std::string& statement);
    // print error messages from error handler
    void error(const std::string& currProcess, int errorCode, const std::string& errorCause);
    // checks if the program ends due to errors - checked at the end of every stage
    bool endProcess(const std::string& currProcess);
    // print warning and error count in entire program
    void endProgram(const std::string& currProcess);
};