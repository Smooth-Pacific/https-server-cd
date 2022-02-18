#include <string>

class Config{
    private:
        Config();

        uint16_t         PORT;
        uint16_t         MAX_CONNECTIONS;
        uint16_t         CONNECTION_TIMEOUT;
        uint16_t         MEMORY_LIMIT;
        uint16_t         MAX_THREADS;
        std::string      HTTPS_MEM_KEY;
        std::string      HTTPS_MEM_CERT;

    public:
        static Config& get_instance();

        void update_config();

        uint16_t         GET_PORT();
        uint16_t         GET_MAX_CONNECTIONS();
        uint16_t         GET_CONNECTION_TIMEOUT();
        uint16_t         GET_MEMORY_LIMIT();
        uint16_t         GET_MAX_THREADS();
        std::string      GET_HTTPS_MEM_KEY();
        std::string      GET_HTTPS_MEM_CERT();
};