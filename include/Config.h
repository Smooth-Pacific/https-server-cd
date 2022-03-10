#include <string>

#include <Logging.h>
class Config{
    private:
        Config(Logging log);

        Logging log;

        uint16_t         PORT;
        uint16_t         MAX_CONNECTIONS;
        uint16_t         CONNECTION_TIMEOUT;
        uint16_t         MEMORY_LIMIT;
        uint16_t         MAX_THREADS;
        std::string      HTTPS_MEM_KEY;
        std::string      HTTPS_MEM_CERT;
        bool             DEBUG_FLAG;
        std::string      NONCE_SEED;

        template <typename T>
        T update_option(T& option, const char* env_var);

        template <typename T, unsigned int base>
        T update_option(T& option, const char* env_var);

    public:
        static Config& get_instance(Logging log);

        void update_config();

        uint16_t         GET_PORT();
        uint16_t         GET_MAX_CONNECTIONS();
        uint16_t         GET_CONNECTION_TIMEOUT();
        uint16_t         GET_MEMORY_LIMIT();
        uint16_t         GET_MAX_THREADS();
        std::string      GET_HTTPS_MEM_KEY();
        std::string      GET_HTTPS_MEM_CERT();
        std::string      GET_NONCE_SEED();
};