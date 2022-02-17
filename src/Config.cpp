#include "Config.h"
#include <thread>

Config::Config(){
    update_config();
}

Config& Config::get_instance(){
    static Config config;
    return config;
}

void Config::update_config(){
    PORT                = getenv("PORT") ? static_cast<uint16_t>(std::stoi(getenv("PORT"))) : 8080;
    HTTPS_MEM_KEY       = getenv("HTTPS_MEM_KEY_PATH") ? static_cast<std::string>(getenv("HTTPS_MEM_KEY_PATH")) : 
                            "../certs/server_ca/private/smoothstack_server.key";
    HTTPS_MEM_CERT      = getenv("HTTPS_MEM_CERT_PATH") ? static_cast<std::string>(getenv("HTTPS_MEM_CERT_PATH")) : 
                            "../certs/server_ca/certs/smoothstack_server.crt";
    MAX_CONNECTIONS     = getenv("MAX_CONNECTIONS") ? static_cast<uint16_t>(std::stoi(getenv("MAX_CONNECTIONS"))) : 4;
    CONNECTION_TIMEOUT  = getenv("TIMEOUT") ? static_cast<uint16_t>(std::stoi(getenv("TIMEOUT"))) : 180;
    MEMORY_LIMIT        = getenv("MEMORY_LIMIT") ? static_cast<uint16_t>(std::stoi(getenv("MEMORY_LIMIT"))) : 32768;
    MAX_THREADS         = getenv("MAX_THREADS") ? static_cast<uint16_t>(std::stoi(getenv("MAX_THREADS"))) : 1;
}

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