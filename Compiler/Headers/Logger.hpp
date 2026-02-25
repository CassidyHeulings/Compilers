#include <string>

class Logger {
private:
    bool debugOn;
public:
    Logger(bool isDebugOn);
    void debug(std::string currProcess, const std::string& statement);
    void info(std::string currProcess, const std::string& statement);
};