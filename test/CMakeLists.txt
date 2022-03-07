project(tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(../include)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(test test.cpp)
target_link_libraries(test PRIVATE config gtest_main)

add_executable(setup setup.cpp)
target_link_libraries(setup PRIVATE logging)

find_package(CURL REQUIRED) 
add_executable(latency latency.cpp)
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(latency ${CURL_LIBRARIES})

add_executable(request request.cpp)
target_link_libraries(request ${CURL_LIBRARIES} ${Boost_LIBRARIES})