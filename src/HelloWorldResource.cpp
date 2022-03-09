#include <chrono>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <unistd.h>

#include <HelloWorldResource.h>
#include <Logging.h>

Logging logger;

const std::shared_ptr<httpserver::http_response> hello_world_resource::render(const httpserver::http_request& req){
    std::string ip_addr = req.http_request::get_requestor();
    
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

    std::shared_ptr<httpserver::string_response> my_response = 
        std::shared_ptr<httpserver::string_response>(new httpserver::string_response(ip_addr, 200, "text/plain"));

    return my_response;
}

const std::shared_ptr<httpserver::http_response> file_resource::render(const httpserver::http_request& req){
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::string ip_addr = req.http_request::get_requestor();

    bool reload_nonce = false;

    if(req.check_digest_auth("test@example.com", "mypass", 300, &reload_nonce)){
        if(req.get_arg("arg1") == "example.xml"){
            
            return std::shared_ptr<httpserver::file_response>(new httpserver::file_response("files/example.xml", 200, "text/xml"));
        }
        else{
            return std::shared_ptr<httpserver::string_response>(
                new httpserver::string_response("Resource not available", 404, "text/plain")
            );
        }
    }
    else if(req.get_digested_user() == ""){
        return std::shared_ptr<httpserver::digest_auth_fail_response>(
            new httpserver::digest_auth_fail_response("No user supplied", "test@example.com", MY_OPAQUE)
        );
    }
    else{
        return std::shared_ptr<httpserver::digest_auth_fail_response>(
            new httpserver::digest_auth_fail_response("Failed to authenticate", "test@example.com", MY_OPAQUE, reload_nonce)
        );
    }
}