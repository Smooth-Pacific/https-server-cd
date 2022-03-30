#include <chrono>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <thread>

#include <Resource.h>
#include <Logging.h>

const std::shared_ptr<httpserver::http_response> hello_world_resource::render(const httpserver::http_request& req){
    Logging log;
    std::string ip_addr = req.http_request::get_requestor();
    
    if(req.get_digested_user() == ""){
        return std::shared_ptr<httpserver::digest_auth_fail_response>(
            new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE, true)
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
    std::string ip_addr = req.http_request::get_requestor();
    bool reload_nonce = false;
    std::stringstream ss;
    Logging log;
    
    //std::this_thread::sleep_for(std::chrono::milliseconds(250));

    // checks if user is provided and password exists
    if(req.check_digest_auth("test@example.com", "mypass", 300, &reload_nonce)){
        // checks if file exists
        if(req.get_arg("arg1") == "example.xml"){
            ss << ip_addr << " 200 - GET \'files/example.xml\'";
            log.log(Logging::severity_level::info, ss.str(), "SERVER");

            return std::shared_ptr<httpserver::file_response>(
                new httpserver::file_response("files/example.xml", 200, "text/xml")
            );
        }
        // if file does not exist, 404
        else{
            ss << ip_addr << " 404 - GET \'files/example.xml\'";
            log.log(Logging::severity_level::info, ss.str(), "SERVER");

            return std::shared_ptr<httpserver::string_response>(
                new httpserver::string_response("Resource not available", 404, "text/plain")
            );
        }
    }
    // check if request has provided a user
    else if(req.get_digested_user() == ""){
        ss << ip_addr << " 401 - GET \'files/example.xml\' - No user supplied";
        log.log(Logging::severity_level::info, ss.str(), "SERVER");
    
        return std::shared_ptr<httpserver::digest_auth_fail_response>(
            new httpserver::digest_auth_fail_response("No user supplied", "test@example.com", MY_OPAQUE)
        );
    }
    // if no user or password is provided
    else{
        ss << ip_addr << " 401 - GET \'files/example.xml\' - Failed to authenticate";
        log.log(Logging::severity_level::info, ss.str(), "SERVER");
        
        return std::shared_ptr<httpserver::digest_auth_fail_response>(
            new httpserver::digest_auth_fail_response("Failed to authenticate", "test@example.com", MY_OPAQUE, reload_nonce)
        );
    }
}