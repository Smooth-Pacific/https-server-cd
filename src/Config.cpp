#include <Config.h>
//#include <Logging.h>

#include <thread>
#include <iostream>
#include <cstring>
#include <sstream>

Config::Config(Logging log): 
    log(log),
    PORT(8080), 
    MAX_CONNECTIONS(32), 
    CONNECTION_TIMEOUT(180), 
    MEMORY_LIMIT(32768),
    MAX_THREADS(1), 
    HTTPS_MEM_KEY("./certs/server_ca/private/smoothstack_server.key"),
    HTTPS_MEM_CERT("./certs/server_ca/certs/smoothstack_server.crt"),
    NONCE_SEED("Default_nonce_seed"){

    // update values if environment variables are set
    update_config();

    std::stringstream ss;

    ss << "Server option Port:                  "<< GET_PORT();
    log.log(Logging::severity_level::info, ss.str(), "SERVER");
    ss.str("");

    ss << "Server option HTTPS_MEM_KEY:         "<< GET_HTTPS_MEM_KEY();
    log.log(Logging::severity_level::info, ss.str(), "SERVER");
    ss.str("");

    ss << "Server option HTTPS_MEM_CERT:        "<< GET_HTTPS_MEM_CERT();
    log.log(Logging::severity_level::info, ss.str(), "SERVER");
    ss.str("");

    ss << "Server option MAX_CONNECTIONS:       "<< GET_MAX_CONNECTIONS();
    log.log(Logging::severity_level::info, ss.str(), "SERVER");
    ss.str("");

    ss << "Server option CONNECTION_TIMEOUT:    "<< GET_CONNECTION_TIMEOUT();
    log.log(Logging::severity_level::info, ss.str(), "SERVER");
    ss.str("");

    ss << "Server option MEMORY_LIMIT:          "<< GET_MEMORY_LIMIT();
    log.log(Logging::severity_level::info, ss.str(), "SERVER");
    ss.str("");

    ss << "Server option MAX_THREADS:           "<< GET_MAX_THREADS();
    log.log(Logging::severity_level::info, ss.str(), "SERVER");
    ss.str("");

    ss << "Server option NONCE_SEED:            "<< GET_NONCE_SEED();
    log.log(Logging::severity_level::info, ss.str(), "SERVER");
    ss.str("");

}

template <typename T>
T Config::update_option(T& option, const char* env_var){
    char* buffer = getenv(env_var);
    if(buffer != NULL){
        return static_cast<T>(getenv(env_var));
    }
    else{
        return option;
    }
}

template <typename T, unsigned int base>
T Config::update_option(T& option, const char* env_var){
    std::stringstream ss;
    char* buffer = getenv(env_var);
    try{
        if(buffer != NULL){
            int env_num = std::stoi(buffer);
            if(env_num > 0 && env_num < 65536){
                return static_cast<T>(std::stoul(buffer, nullptr, base));
            }
            else{
                throw(buffer);
            }
        }
        else{
            return option;
        }
    }
    catch(const char* e){
        ss << env_var << " - " << e << " out of bounds";
        log.log(Logging::severity_level::critical, ss.str(), "SERVER");
        std::cerr << "ERROR: " << env_var << " - " << e << " out of bounds" << std::endl;
        exit(1);
    }
}

Config& Config::get_instance(Logging log){
    static Config config(log);
    return config;
}
// #### PUBLIC METHODS START HERE #### //

// this should only ever be used for testing purposes outside of the class
void Config::update_config(){
    PORT                = update_option<uint16_t, 10>   (PORT, "PORT");
    HTTPS_MEM_KEY       = update_option<std::string>    (HTTPS_MEM_KEY, "HTTPS_MEM_KEY_PATH");
    HTTPS_MEM_CERT      = update_option<std::string>    (HTTPS_MEM_CERT, "HTTPS_MEM_CERT_PATH");
    MAX_CONNECTIONS     = update_option<uint16_t, 10>   (MAX_CONNECTIONS, "MAX_CONNECTIONS");
    CONNECTION_TIMEOUT  = update_option<uint16_t, 10>   (CONNECTION_TIMEOUT, "CONNECTION_TIMEOUT");
    MEMORY_LIMIT        = update_option<uint16_t, 10>   (MEMORY_LIMIT, "MEMORY_LIMIT");
    MAX_THREADS         = update_option<uint16_t, 10>   (MAX_THREADS, "MAX_THREADS");
    NONCE_SEED          = update_option<std::string>    (NONCE_SEED, "NONCE_SEED");
}

// getters
uint16_t Config::GET_PORT(){
    return PORT;
}
uint16_t Config::GET_MAX_CONNECTIONS(){
    return MAX_CONNECTIONS;
}
uint16_t Config::GET_CONNECTION_TIMEOUT(){
    return CONNECTION_TIMEOUT;
}
uint16_t Config::GET_MEMORY_LIMIT(){
    return MEMORY_LIMIT;
}
std::string Config::GET_HTTPS_MEM_KEY(){
    return HTTPS_MEM_KEY;
}
std::string Config::GET_HTTPS_MEM_CERT(){
    return HTTPS_MEM_CERT;
}
uint16_t Config::GET_MAX_THREADS(){
    // if more than 4 cores is set, start a thread pool (>1 cores automatically starts a thread pool)
    const int processor_count = std::thread::hardware_concurrency();
    if(MAX_THREADS >= 4 && processor_count >= 4){
        return MAX_THREADS;
    }
    return 1;
}
std::string Config::GET_NONCE_SEED(){
    return NONCE_SEED;
}