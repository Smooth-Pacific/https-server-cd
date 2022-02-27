#include <string>
#include <ostream>
#include <fstream>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

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
        void log_trace(const std::string& msg, const std::string& filter = "");
};