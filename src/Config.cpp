#include "Config.h"

Config::Config(){
    PORT = getenv("PORT") ? static_cast<int>(std::stoi(getenv("PORT"))) : 8080;

    HTTPS_MEM_KEY = getenv("HTTPS_MEM_KEY_PATH") ? static_cast<std::string>(getenv("HTTPS_MEM_KEY_PATH")) : 
        "../certs/server_ca/private/smoothstack_server.key";

    HTTPS_MEM_CERT = getenv("HTTPS_MEM_CERT_PATH") ? static_cast<std::string>(getenv("HTTPS_MEM_CERT_PATH")) : 
        "../certs/server_ca/certs/smoothstack_server.crt";

    MAX_CONNECTIONS = getenv("MAX_CONNECTIONS") ? static_cast<int>(std::stoi(getenv("MAX_CONNECTIONS"))) : 4;

    CONNECTION_TIMEOUT = getenv("TIMEOUT") ? static_cast<int>(std::stoi(getenv("TIMEOUT"))) : 180;

    MEMORY_LIMIT = getenv("MEMORY_LIMIT") ? static_cast<int>(std::stoi(getenv("MEMORY_LIMIT"))) : 32768;
}

Config& Config::get_instance(){
    static Config config;
    return config;
}