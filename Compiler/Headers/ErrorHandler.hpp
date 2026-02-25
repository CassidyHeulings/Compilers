#include <string>

// returns error codes to logger
class ErrorHandler {
public:
    ErrorHandler();
    std::string getErrorMessage(int errorCode);
private:
    std::string flags[2] = {
        "Character not used in grammar", // error code 0
        "Not a valid token" // error code 1
    };
};