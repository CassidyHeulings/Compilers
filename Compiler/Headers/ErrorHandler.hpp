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
    // reset the current program counters
    void resetCounters();
    // get the number of errors in current program
    int getNumErrors();
    // get number of warnings in current program
    int getNumWarnings();
    // get number of errors in all programs
    int getNumAllErrors();
    // get number of warnings in all programs
    int getNumAllWarnings();
private:
    // number of errors in the current program
    int errorCounter;
    // number of warnings in the current program
    int warningCounter;
    // number of errors in all programs
    int allErrorCounter;
    // number of warnings in all programs
    int allWarningCounter;
    // possible errors
    std::string eflags[3] = {
        "Invalid symbol in grammar", // error code 0
        "Invalid token", // error code 1
        "Invalid grammar" // error code 2
    };
    std::string wflags[5] = {
        "Comment never closed", // warning code 0
        "Program was not ended properly", // warning code 1
        "No code inputted", // warning code 2
        "Parse tree ends prematurely after first error", // warning code 3
        "Incomplete code" // warning code 4
    };
};