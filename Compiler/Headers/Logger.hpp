#include <string>

class Logger {
private:
    bool debugOn;
public:
    Logger(bool isDebugOn);
    void debug(const std::string& statement);
    void info(const std::string& statement);
};