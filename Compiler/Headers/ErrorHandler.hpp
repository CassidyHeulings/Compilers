#include <string>
#pragma once

// returns error codes to logger
class ErrorHandler {
public:
    ErrorHandler();
    // return the error message based on the code, increase error count
    std::string getErrorMessage(int errorCode);
    // return the warning message based on the code, increase warning count
    std::string getWarningMessage(int warningCode);
    // get the number of errors in the current process
    int getNumErrors();
    // get number of warnings in entire program
    int getNumWarnings();
private:
    // number of errors in the current process
    int errorCounter;
    // number of warnings in the program
    int warningCounter;
    // possible errors
    std::string eflags[2] = {
        "Invalid symbol in grammar", // error code 0
        "Invalid token" // error code 1
    };
    std::string wflags[3] = {
        "Comment never closed", // warning code 0
        "Program was not ended properly", // warning code 1
        "No code inputted" // warning code 2
    };
};