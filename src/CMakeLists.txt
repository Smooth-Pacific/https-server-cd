include_directories(../include)
add_library(config STATIC Config.cpp)

add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} httpserver microhttpd config)
