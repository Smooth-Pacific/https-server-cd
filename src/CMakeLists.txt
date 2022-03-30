include_directories(../include)

add_library(config STATIC Config.cpp)
target_link_libraries(config logging)

add_library(logging STATIC Logging.cpp)
target_link_libraries(logging ${Boost_LIBRARIES})

add_library(performance STATIC Performance.cpp)
target_link_libraries(performance ${Boost_LIBRARIES} logging)

add_library(resource STATIC Resource.cpp)


add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} httpserver microhttpd config performance resource)
