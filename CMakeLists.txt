cmake_minimum_required(VERSION 3.6)

# The project name
project(https)
set(TARGET "server")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#find_package(GTest REQUIRED)
#include(FindProtobuf)
#find_package(Protobuf REQUIRED)
#include_directories(${PROTOBUF_INCLUDE_DIR})

## Include directories
include_directories(${TARGET} ${CMAKE_SOURCE_DIR}/include)
include_directories(${TARGET} ${CMAKE_SOURCE_DIR}/test)
set(INCLUDE_DIR include)

## Source directories
set(SOURCE_DIR src)

## Add CXX sources
set(SOURCES
    # CXX sources
    ${SOURCE_DIR}/main.cpp
    # CXX includes
    #${INCLUDE_DIR}/Include.hpp
)

## Set output directory
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Bin)

## Application executable and link dependencies
add_executable(${TARGET} ${SOURCES})

## Add link directories
#target_link_directories(${TARGET} PRIVATE /usr/local/lib)

## Link Dependencies
target_link_libraries(${TARGET} httpserver microhttpd)