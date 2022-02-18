#include <iostream>
#include <gtest/gtest.h>
#include <stdlib.h>

#include "Config.h"

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Config_, Default_Values){
    Config& config = Config::get_instance();

    ASSERT_EQ(config.GET_PORT(), 8080);
    ASSERT_EQ(config.GET_MAX_CONNECTIONS(), 4);
    ASSERT_EQ(config.GET_CONNECTION_TIMEOUT(), 180);
    ASSERT_EQ(config.GET_MEMORY_LIMIT(), 32768);
    ASSERT_EQ(config.GET_HTTPS_MEM_KEY(), "../certs/server_ca/private/smoothstack_server.key");
    ASSERT_EQ(config.GET_HTTPS_MEM_CERT(), "../certs/server_ca/certs/smoothstack_server.crt");
    ASSERT_EQ(config.GET_MAX_THREADS(), 1);
}

TEST(Config_, Ports){
    setenv("PORT", "8081", 0);
    setenv("MAX_CONNECTIONS", "16", 0);
    setenv("TIMEOUT", "120", 0);
    setenv("MEMORY_LIMIT", "65535", 0);
    setenv("HTTPS_MEM_KEY_PATH", "~/certs/server_ca/private/smoothstack_server.key", 0);
    setenv("HTTPS_MEM_CERT_PATH", "~/certs/server_ca/certs/smoothstack_server.crt", 0);
    setenv("MAX_THREADS", "8", 0);

    Config& config = Config::get_instance();
    config.update_config();

    ASSERT_EQ(config.GET_PORT(), 8081);
    ASSERT_EQ(config.GET_MAX_CONNECTIONS(), 16);
    ASSERT_EQ(config.GET_CONNECTION_TIMEOUT(), 120);
    ASSERT_EQ(config.GET_MEMORY_LIMIT(), 65535);
    ASSERT_EQ(config.GET_HTTPS_MEM_KEY(), "~/certs/server_ca/private/smoothstack_server.key");
    ASSERT_EQ(config.GET_HTTPS_MEM_CERT(), "~/certs/server_ca/certs/smoothstack_server.crt");
    ASSERT_EQ(config.GET_MAX_THREADS(), 8);
}