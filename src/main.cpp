#include <httpserver.hpp>
#include <iostream>
#include <iomanip>
#include <cstdlib>

class hello_world_resource : public httpserver::http_resource {
public:
    const std::shared_ptr<httpserver::http_response> render(const httpserver::http_request&) {
        return std::shared_ptr<httpserver::http_response>(new httpserver::string_response("Hello, World!"));
    }
};

int main(int argc, char** argv) {
    // get environment veriables
    int port = getenv("PORT") ? static_cast<int>(std::stoi(getenv("PORT"))) : 8080;
    std::string https_mem_key = getenv("HTTPS_MEM_KEY_PATH") ? static_cast<std::string>(getenv("HTTPS_MEM_KEY_PATH")) : 
        "../certs/server_ca/private/smoothstack_server.key";
    std::string https_mem_cert = getenv("HTTPS_MEM_CERT_PATH") ? static_cast<std::string>(getenv("HTTPS_MEM_CERT_PATH")) : 
        "../certs/server_ca/certs/smoothstack_server.crt";
    int max_connections = getenv("MAX_CONNECTIONS") ? static_cast<int>(std::stoi(getenv("MAX_CONNECTIONS"))) : 4;
    int connection_timeout = getenv("TIMEOUT") ? static_cast<int>(std::stoi(getenv("TIMEOUT"))) : 180;
    int memory_limit = getenv("MEMORY_LIMIT") ? static_cast<int>(std::stoi(getenv("MEMORY_LIMIT"))) : 32768;

    httpserver::webserver ws = httpserver::create_webserver(port)
        .use_ssl()
        .https_mem_key(https_mem_key)
        .https_mem_cert(https_mem_cert)
        .max_connections(max_connections)
        .connection_timeout(connection_timeout)
        .memory_limit(memory_limit)
        .use_dual_stack()
        .start_method(httpserver::http::http_utils::INTERNAL_SELECT);

    hello_world_resource hwr;
    ws.register_resource("/helloworld", &hwr);
    ws.start(true);

    return 0;
}