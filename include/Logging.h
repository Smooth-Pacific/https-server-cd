#include <string>
#include <ostream>
#include <fstream>

class Logging{
    private:
        bool auto_flush;

    public:
        Logging();
        void init();
        
        enum severity_level{
            trace,
            info,
            normal,
            notification,
            warning,
            error,
            critical
        };
        
        // logging methods
        void log(severity_level sl, const std::string& msg, const std::string& filter = "");
};