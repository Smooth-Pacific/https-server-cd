#include <string>

class Config{
    private:
        Config();

    public:
        int         PORT;
        int         MAX_CONNECTIONS;
        int         CONNECTION_TIMEOUT;
        int         MEMORY_LIMIT;
        std::string HTTPS_MEM_KEY;
        std::string HTTPS_MEM_CERT;

        static Config& get_instance();
};