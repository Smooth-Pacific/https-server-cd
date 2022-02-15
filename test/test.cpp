#include <iostream>

#include <gtest/gtest.h>
#include "Config.h"

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Config_, Config_Default_Values){
    Config& config = Config::get_instance();

    ASSERT_EQ(config.GET_PORT(), 8080);
    ASSERT_EQ(config.GET_MAX_CONNECTIONS(), 4);
    ASSERT_EQ(config.GET_CONNECTION_TIMEOUT(), 180);
    ASSERT_EQ(config.GET_MEMORY_LIMIT(), 32768);
    ASSERT_EQ(config.GET_HTTPS_MEM_KEY(), "../certs/server_ca/private/smoothstack_server.key");
    ASSERT_EQ(config.GET_HTTPS_MEM_CERT(), "../certs/server_ca/certs/smoothstack_server.crt");
    ASSERT_EQ(config.GET_MAX_THREADS(), 1);
}