#include <httpserver.hpp>
#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "Config.h"

class hello_world_resource : public httpserver::http_resource {
public:
    const std::shared_ptr<httpserver::http_response> render(const httpserver::http_request&) {
        return std::shared_ptr<httpserver::http_response>(new httpserver::string_response("Hello, World!"));
    }
};

int main(int argc, char** argv) {
    Config& config = Config::get_instance();  

    httpserver::webserver ws = httpserver::create_webserver(config.PORT)
        .use_ssl()
        .https_mem_key(config.HTTPS_MEM_KEY)
        .https_mem_cert(config.HTTPS_MEM_CERT)
        .max_connections(config.MAX_CONNECTIONS)
        .connection_timeout(config.CONNECTION_TIMEOUT)
        .memory_limit(config.MEMORY_LIMIT)
        .use_dual_stack()
        .start_method(httpserver::http::http_utils::INTERNAL_SELECT);

    hello_world_resource hwr;
    ws.register_resource("/helloworld", &hwr);
    ws.start(true);

    return 0;
}