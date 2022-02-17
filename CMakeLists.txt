cmake_minimum_required(VERSION 3.6)

# The project name
project(https)
set(TARGET "server")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_ALL_DYN_LINK -pthread -g" CACHE STRING "compile flags" FORCE)
#add_compile_options(-pthread -g)

find_package(GTest REQUIRED)
find_package(Boost COMPONENTS log log_setup REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

## add directories
add_subdirectory(src)
add_subdirectory(test)