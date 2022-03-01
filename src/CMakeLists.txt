include_directories(../include)

add_library(config STATIC Config.cpp)

add_library(logging STATIC Logging.cpp)
target_link_libraries(logging ${Boost_LIBRARIES})

add_library(performance STATIC Performance.cpp)
target_link_libraries(performance ${Boost_LIBRARIES} logging)

add_library(hello_world_resource STATIC HelloWorldResource.cpp)


add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} httpserver microhttpd config performance hello_world_resource)
