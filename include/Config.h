#include <string>

class Config{
    private:
        Config();

        int         PORT;
        int         MAX_CONNECTIONS;
        int         CONNECTION_TIMEOUT;
        int         MEMORY_LIMIT;
        int         MAX_THREADS;
        std::string HTTPS_MEM_KEY;
        std::string HTTPS_MEM_CERT;

    public:
        static Config& get_instance();

        // getters
        int         GET_PORT();
        int         GET_MAX_CONNECTIONS();
        int         GET_CONNECTION_TIMEOUT();
        int         GET_MEMORY_LIMIT();
        int         GET_MAX_THREADS();
        std::string GET_HTTPS_MEM_KEY();
        std::string GET_HTTPS_MEM_CERT();
};