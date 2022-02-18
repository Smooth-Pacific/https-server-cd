include_directories(../include)
add_library(config STATIC Config.cpp)
add_library(performance STATIC Performance.cpp)
target_link_libraries(performance ${Boost_LIBRARIES})

add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} httpserver microhttpd config performance)
