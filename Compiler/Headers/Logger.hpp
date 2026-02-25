#include <string>
#include "ErrorHandler.hpp"

class Logger {
private:
    bool debugOn;
    ErrorHandler& errorHandler;
public:
    Logger(ErrorHandler& errorHandlerInstance, bool isDebugOn);
    // print debug and info messages from main compiler
    void debug(std::string currProcess, const std::string& statement);
    void info(std::string currProcess, const std::string& statement);
    // print error messages from error handler
    void error(std::string currProcess, int errorCode, std::string& errorCause);
};