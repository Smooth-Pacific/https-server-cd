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

    httpserver::webserver ws = httpserver::create_webserver(config.GET_PORT())
        .use_ssl()
        .https_mem_key(config.GET_HTTPS_MEM_KEY())
        .https_mem_cert(config.GET_HTTPS_MEM_CERT())
        .max_connections(config.GET_MAX_CONNECTIONS())
        .connection_timeout(config.GET_CONNECTION_TIMEOUT())
        .memory_limit(config.GET_MEMORY_LIMIT())
        .use_dual_stack()
        .start_method(httpserver::http::http_utils::INTERNAL_SELECT)
        .max_threads(config.GET_MAX_THREADS());

    hello_world_resource hwr;
    ws.register_resource("/helloworld", &hwr);
    ws.start(true);

    return 0;
}