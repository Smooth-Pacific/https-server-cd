#include <chrono>
#include <sstream>
#include <iostream>

#include <HelloWorldResource.h>
#include <Logging.h>

Logging logger;

const std::shared_ptr<httpserver::http_response> hello_world_resource::render(const httpserver::http_request& req){
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    // get requestor IP address; does nothing for now but I will probably log accesses
    std::string ip_addr = req.http_request::get_requestor();

    // build latency log message; log is in micro seconds
    std::stringstream ss;
    
    if(req.get_digested_user() == ""){
        return std::shared_ptr<httpserver::digest_auth_fail_response>(
            new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE)
        );
    }
    else{
        bool reload_nonce = false;

        if(!req.check_digest_auth("test@example.com", "mypass", 300, &reload_nonce)){
            return std::shared_ptr<httpserver::digest_auth_fail_response>(
                new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE, reload_nonce)
            );
        }
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    ss << ip_addr << ";" << std::chrono::duration_cast<std::chrono::microseconds>(end-start).count();
    logger.log_trace(ss.str(), "LATENCY");

    return std::shared_ptr<httpserver::string_response>(new httpserver::string_response(
        ip_addr, 200, "text/plain"
    ));
}