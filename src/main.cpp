#include <httpserver.hpp>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <thread>

#include "Config.h"
#include "Performance.h"
#include "HelloWorldResource.h"

void custom_access_log(const std::string& url){
    // I will probably log something here when clients connect
}

int main(int argc, char** argv) {
    Config& config = Config::get_instance();
    Performance_Monitoring pm;
    std::thread t(&Performance_Monitoring::run, pm);

    httpserver::webserver ws = httpserver::create_webserver(config.GET_PORT())
        .use_ssl()
        .https_mem_key(config.GET_HTTPS_MEM_KEY())
        .https_mem_cert(config.GET_HTTPS_MEM_CERT())
        .max_connections(config.GET_MAX_CONNECTIONS())
        .connection_timeout(config.GET_CONNECTION_TIMEOUT())
        .memory_limit(config.GET_MEMORY_LIMIT())
        .use_dual_stack()
        .start_method(httpserver::http::http_utils::INTERNAL_SELECT)
        .max_threads(config.GET_MAX_THREADS())
        .log_access(custom_access_log)
        .digest_auth();
        
    hello_world_resource hwr;
    ws.register_resource("/helloworld", &hwr);
    ws.start(true);

    return 0;
}