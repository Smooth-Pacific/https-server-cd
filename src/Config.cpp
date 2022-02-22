#include "Config.h"
#include <thread>
#include <iostream>
#include <iomanip>

Config::Config(): 
    PORT(8080), 
    MAX_CONNECTIONS(4), 
    CONNECTION_TIMEOUT(180), 
    MEMORY_LIMIT(32768),
    MAX_THREADS(1), 
    HTTPS_MEM_KEY("./certs/server_ca/private/smoothstack_server.key"),
    HTTPS_MEM_CERT("./certs/server_ca/certs/smoothstack_server.crt"){

    // update values if environment variables are set
    update_config();

    std::cout << "Server option Port:            " << GET_PORT() << std::endl;
    std::cout << "Server option HTTPS_MEM_KEY:   " << GET_HTTPS_MEM_KEY() << std::endl;
    std::cout << "Server option HTTPS_MEM_CERT:  " << GET_HTTPS_MEM_CERT() << std::endl;
    std::cout << "Server option MAX_CONNECTIONS: " << GET_MAX_CONNECTIONS() << std::endl;
    std::cout << "Server option TIMEOUT:         " << GET_CONNECTION_TIMEOUT() << std::endl;
    std::cout << "Server option MEMORY_LIMIT:    " << GET_MEMORY_LIMIT() << std::endl;
    std::cout << "Server option MAX_THREADS:     " << GET_MAX_THREADS() << std::endl;
}

template <typename T>
T Config::update_option(T& option, const char* env_var){
    char* buffer = getenv(env_var);
    if(buffer != NULL){
        return static_cast<T>(getenv(env_var));
    }
    else{
        std::cout << option << std::endl;
        return option;
    }
}

template <typename T, unsigned int base>
T Config::update_option(T& option, const char* env_var){
    char* buffer = getenv(env_var);
    if(buffer != NULL){
        return static_cast<T>(std::stoul(getenv(env_var), nullptr, base));
    }
    else{
        return option;
    }
}

Config& Config::get_instance(){
    static Config config;
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
    if(MAX_THREADS > 4 && processor_count > 4){
        return MAX_THREADS;
    }
    return 1;
}