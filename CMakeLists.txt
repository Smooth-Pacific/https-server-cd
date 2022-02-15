cmake_minimum_required(VERSION 3.6)

# The project name
project(https)
set(TARGET "server")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest REQUIRED)

## add directories
add_subdirectory(src)
add_subdirectory(test)

#add_library(config STATIC src/Config.cpp include/Config.h)

## Link Dependencies
#target_link_libraries(${TARGET} httpserver microhttpd config)