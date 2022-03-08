#include <iostream>
#include <gtest/gtest.h>
#include <stdlib.h>

#include "Config.h"

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Config_Class, Default_Values){
    Config& config = Config::get_instance(false);

    ASSERT_EQ(config.GET_PORT(), 8080);
    ASSERT_EQ(config.GET_MAX_CONNECTIONS(), 32);
    ASSERT_EQ(config.GET_CONNECTION_TIMEOUT(), 180);
    ASSERT_EQ(config.GET_MEMORY_LIMIT(), 32768);
    ASSERT_EQ(config.GET_HTTPS_MEM_KEY(), "./certs/server_ca/private/smoothstack_server.key");
    ASSERT_EQ(config.GET_HTTPS_MEM_CERT(), "./certs/server_ca/certs/smoothstack_server.crt");
    ASSERT_EQ(config.GET_MAX_THREADS(), 1);
}

TEST(Config_Class, Ports){
    Config& config = Config::get_instance(false);

    //setenv("PORT", "0", 1);
    //config.update_config();
    //ASSERT_EQ(config.GET_PORT(), 8080);
//
    //setenv("PORT", "65536", 1);
    //config.update_config();
    //ASSERT_EQ(config.GET_PORT(), 8080);

    setenv("PORT", "8081", 1);
    config.update_config();
    ASSERT_EQ(config.GET_PORT(), 8081);

    setenv("PORT", "65535", 1);
    config.update_config();
    ASSERT_EQ(config.GET_PORT(), 65535);

    setenv("PORT", "8080", 1);
    config.update_config();
    ASSERT_EQ(config.GET_PORT(), 8080);
}

TEST(Config_Class, Max_connections){
    Config& config = Config::get_instance(false);

    //setenv("MAX_CONNECTIONS", "0", 1);
    //config.update_config();
    //ASSERT_EQ(config.GET_MAX_CONNECTIONS(), 32);

    setenv("MAX_CONNECTIONS", "32", 1);
    config.update_config();
    ASSERT_EQ(config.GET_MAX_CONNECTIONS(), 32);

    setenv("MAX_CONNECTIONS", "65535", 1);
    config.update_config();
    ASSERT_EQ(config.GET_MAX_CONNECTIONS(), 65535);

    setenv("MAX_CONNECTIONS", "16", 1);
    config.update_config();
    ASSERT_EQ(config.GET_MAX_CONNECTIONS(), 16);
}

TEST(Config_Class, Connection_timeout){
    Config& config = Config::get_instance(false);

    //setenv("CONNECTION_TIMEOUT", "0", 1);
    //config.update_config();
    //ASSERT_EQ(config.GET_CONNECTION_TIMEOUT(), 180);

    setenv("CONNECTION_TIMEOUT", "120", 1);
    config.update_config();
    ASSERT_EQ(config.GET_CONNECTION_TIMEOUT(), 120);

    setenv("CONNECTION_TIMEOUT", "65535", 1);
    config.update_config();
    ASSERT_EQ(config.GET_CONNECTION_TIMEOUT(), 65535);

    setenv("CONNECTION_TIMEOUT", "120", 1);
    config.update_config();
    ASSERT_EQ(config.GET_CONNECTION_TIMEOUT(), 120);
}

TEST(Config_Class, Memory_limit){
    Config& config = Config::get_instance(false);

    //setenv("MEMORY_LIMIT", "0", 1);
    //config.update_config();
    //ASSERT_EQ(config.GET_MEMORY_LIMIT(), 32768);

    setenv("MEMORY_LIMIT", "65535", 1);
    config.update_config();
    ASSERT_EQ(config.GET_MEMORY_LIMIT(), 65535);

    setenv("MEMORY_LIMIT", "12345", 1);
    config.update_config();
    ASSERT_EQ(config.GET_MEMORY_LIMIT(), 12345);

    setenv("MEMORY_LIMIT", "120", 1);
    config.update_config();
    ASSERT_EQ(config.GET_MEMORY_LIMIT(), 120);
}

TEST(Config_Class, Max_threads){
    Config& config = Config::get_instance(false);

    //setenv("MAX_THREADS", "0", 1);
    //config.update_config();
    //ASSERT_EQ(config.GET_MAX_THREADS(), 1);

    setenv("MAX_THREADS", "65535", 1);
    config.update_config();
    ASSERT_EQ(config.GET_MAX_THREADS(), 65535);

    setenv("MAX_THREADS", "12345", 1);
    config.update_config();
    ASSERT_EQ(config.GET_MAX_THREADS(), 12345);

    setenv("MAX_THREADS", "8", 1);
    config.update_config();
    ASSERT_EQ(config.GET_MAX_THREADS(), 8);
}

TEST(Config_Class, HTTPS_MEM_KEY_PATH){
    Config& config = Config::get_instance(false);

    setenv("HTTPS_MEM_KEY_PATH", "~/certs/server_ca/private/smoothstack_server.key", 1);
    config.update_config();
    ASSERT_EQ(config.GET_HTTPS_MEM_KEY(), "~/certs/server_ca/private/smoothstack_server.key");

    setenv("HTTPS_MEM_KEY_PATH", "./certs/server_ca/private/smoothstack_server.key", 1);
    config.update_config();
    ASSERT_EQ(config.GET_HTTPS_MEM_KEY(), "./certs/server_ca/private/smoothstack_server.key");

    setenv("HTTPS_MEM_KEY_PATH", "~/certs/server_ca/private/smoothstack_server.key", 1);
    config.update_config();
    ASSERT_EQ(config.GET_HTTPS_MEM_KEY(), "~/certs/server_ca/private/smoothstack_server.key");
}

TEST(Config_Class, HTTPS_MEM_CERT_PATH){
    Config& config = Config::get_instance(false);

    setenv("HTTPS_MEM_CERT_PATH", "~/certs/server_ca/cert/smoothstack_server.crt", 1);
    config.update_config();
    ASSERT_EQ(config.GET_HTTPS_MEM_CERT(), "~/certs/server_ca/cert/smoothstack_server.crt");

    setenv("HTTPS_MEM_CERT_PATH", "./certs/server_ca/cert/smoothstack_server.crt", 1);
    config.update_config();
    ASSERT_EQ(config.GET_HTTPS_MEM_CERT(), "./certs/server_ca/cert/smoothstack_server.crt");

    setenv("HTTPS_MEM_CERT_PATH", "~/certs/server_ca/cert/smoothstack_server.crt", 1);
    config.update_config();
    ASSERT_EQ(config.GET_HTTPS_MEM_CERT(), "~/certs/server_ca/cert/smoothstack_server.crt");
}