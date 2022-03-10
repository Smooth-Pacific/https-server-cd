#include <httpserver.hpp>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <thread>
#include <filesystem>
#include <atomic>
#include <sstream>

#include <Config.h>                 // Config
#include <Performance.h>            // Performance_Monitoring
#include <HelloWorldResource.h>     // hello_world_resource

void custom_access_log(const std::string& url){
    // I will probably log something here when clients connect
}

int main(int argc, char** argv) {
    std::stringstream ss;
    Logging log;
    log.init();

    log.log(Logging::severity_level::info, "HTTP server has started", "SERVER");

    Config& config = Config::get_instance(log);

    std::atomic<bool> stop_thread_flag = false;
    Performance_Monitoring pm(stop_thread_flag);
    std::thread t(&Performance_Monitoring::run, std::ref(pm));

    httpserver::create_webserver cw = httpserver::create_webserver(config.GET_PORT())
        // set required parameters
        .use_ssl()
        .max_connections(config.GET_MAX_CONNECTIONS())
        .connection_timeout(config.GET_CONNECTION_TIMEOUT())
        .memory_limit(config.GET_MEMORY_LIMIT())
        .start_method(httpserver::http::http_utils::INTERNAL_SELECT)
        .max_threads(config.GET_MAX_THREADS())
        .log_access(custom_access_log)
        .use_dual_stack()
        .digest_auth()
        .digest_auth_random(config.GET_NONCE_SEED());

    // check for valid CA key
    if(std::filesystem::exists(config.GET_HTTPS_MEM_KEY())){
        cw.https_mem_key(config.GET_HTTPS_MEM_KEY());
    }
    else{
        ss << "Could not find HTTPS_MEM_KEY: " << config.GET_HTTPS_MEM_KEY();
        log.log(Logging::severity_level::critical, ss.str(), "SERVER");
        stop_thread_flag = true;    // allow for graceful stop of logging thread
        exit(1);
    }

    // check for valid CA cert/crt
    if(std::filesystem::exists(config.GET_HTTPS_MEM_CERT())){
        cw.https_mem_cert(config.GET_HTTPS_MEM_CERT());
    }
    else{
        ss << "Could not find HTTPS_MEM_CERT: " << config.GET_HTTPS_MEM_CERT();
        log.log(Logging::severity_level::critical, ss.str(), "SERVER");
        stop_thread_flag = true;    // allow for graceful stop of logging thread   
        exit(1);
    }

    // create webserver
    httpserver::webserver ws = cw;
    
    // create resource and endpoint
    hello_world_resource hwr;
    file_resource fr;
    ws.register_resource("/helloworld", &hwr);
    ws.register_resource("/file/{arg1}", &fr);

    // start web server
    ws.start(true);

    return 0;
}