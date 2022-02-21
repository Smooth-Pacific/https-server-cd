#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <chrono>
#include <thread>
#include "sys/types.h"
#include "sys/sysinfo.h"

#include "Performance.h"

Performance_Monitoring::Performance_Monitoring(std::atomic<bool>& stop_thread_flag){
    stop_thread_flag = &stop_thread_flag;
    boost::log::add_file_log(
        boost::log::keywords::file_name = "data.log",
        boost::log::keywords::target_file_name = "data.log",
        boost::log::keywords::open_mode = std::ios::app,
        boost::log::keywords::format = "[%TimeStamp%] [%Severity%] %Message%",
        
        // https://stackoverflow.com/questions/18034738/boost-log-file-not-written-to/18036016#18036016
        boost::log::keywords::auto_flush = false     // turn off auto_flush on production (debugging only)
    );

    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
    boost::log::add_common_attributes();
}

void Performance_Monitoring::monitor(){
    while(!stop_thread_flag){
        struct sysinfo memInfo;

        sysinfo (&memInfo);
        // Get total physical RAM
        long long totalPhysMem = memInfo.totalram;
        totalPhysMem /= 1024;

        // Get total physical RAM used
        long long physMemUsed = memInfo.totalram - memInfo.freeram;
        physMemUsed /= 1024;

        BOOST_LOG_TRIVIAL(info) << get_process_ram_usage() << "KB used by the process";
        //BOOST_LOG_TRIVIAL(info) << physMemUsed << "KB / " << totalPhysMem << "KB";

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

uint16_t Performance_Monitoring::parse_line(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    uint16_t i = strlen(line);
    const char* p = line;
    while(*p <'0' || *p > '9'){
        p++;
    }
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

// this returns in KB
uint16_t Performance_Monitoring::get_process_ram_usage(){
    FILE* file = fopen("/proc/self/status", "r");
    uint16_t result = -1;
    char line[128];

    while(fgets(line, 128, file) != NULL){
        if(strncmp(line, "VmRSS:", 6) == 0){
            result = parse_line(line);
            break;
        }
    }
    fclose(file);
    return result;
}

void Performance_Monitoring::run(){
    monitor();
}