#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <string>

#include <Logging.h>

Logging::Logging(std::string path){
    //char buffer = path.c_str();
    boost::log::add_file_log(
        boost::log::keywords::file_name = "data.log",
        boost::log::keywords::target_file_name = "data.log",
        boost::log::keywords::open_mode = std::ios::app,
        boost::log::keywords::format = "[%TimeStamp%] [%Severity%] %Message%",
        
        // turn off auto_flush on production (debugging only)
        // turn on for performance monitoring
        boost::log::keywords::auto_flush = false
    );

    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::trace);
    boost::log::add_common_attributes();
};

void Logging::log_trace(const std::string& msg){
    BOOST_LOG_TRIVIAL(trace) << msg;
}