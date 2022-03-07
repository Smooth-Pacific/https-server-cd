#include <httpserver.hpp>

#define MY_OPAQUE "11733b200778ce33060f31c9af70a870ba96ddd4"

class hello_world_resource : public httpserver::http_resource {
    public:
        const std::shared_ptr<httpserver::http_response> render(const httpserver::http_request& req);
};

class file_resource : public httpserver::http_resource {
    public:
        const std::shared_ptr<httpserver::http_response> render(const httpserver::http_request& req);
};