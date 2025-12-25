cmake_minimum_required(VERSION 3.28)
project(machina VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Build machina tests" ON)

add_library(machina INTERFACE)
target_include_directories(machina INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(BUILD_TESTS)

enable_testing()

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(machina_tests ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_machina.cpp)

target_link_libraries(machina_tests PRIVATE machina gtest_main)

include(GoogleTest)
gtest_discover_tests(machina_tests)

endif()

