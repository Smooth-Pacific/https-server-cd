#include <httpserver.hpp>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <thread>

#include "Config.h"
#include "Performance.h"

#define MY_OPAQUE "11733b200778ce33060f31c9af70a870ba96ddd4"

void custom_access_log(const std::string& url){
    // I will probably log something here when clients connect
}
class hello_world_resource : public httpserver::http_resource {
public:
    const std::shared_ptr<httpserver::http_response> render(const httpserver::http_request& req) {
        if(req.get_digested_user() == ""){
            return std::shared_ptr<httpserver::digest_auth_fail_response>(
                new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE
            ));
        }
        else{
            bool reload_nonce = false;

            if(!req.check_digest_auth("test@example.com", "mypass", 300, &reload_nonce)){
                return std::shared_ptr<httpserver::digest_auth_fail_response>(
                    new httpserver::digest_auth_fail_response("FAIL", "test@example.com", MY_OPAQUE, reload_nonce
                ));
            }
        }
        return std::shared_ptr<httpserver::string_response>(new httpserver::string_response(
            "SUCCESS", 200, "text/plain"
        ));
    }
};

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