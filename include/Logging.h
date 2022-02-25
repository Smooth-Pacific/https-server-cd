#include <string>

class Logging{
    private:

    public:
        Logging(std::string path);

        void log_trace(const std::string& msg);
};