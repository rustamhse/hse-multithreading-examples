cmake_minimum_required(VERSION 3.25)
project(unbuffered_channel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

set(FETCHCONTENT_BASE_DIR "D:/cmake_deps" CACHE PATH "" FORCE)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  googlebenchmark
  URL https://github.com/google/benchmark/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

add_executable(unbuffered_channel_test test.cpp)
target_link_libraries(unbuffered_channel_test PRIVATE Threads::Threads GTest::gtest GTest::gtest_main)

add_executable(unbuffered_channel_benchmark benchmark.cpp)
target_link_libraries(unbuffered_channel_benchmark PRIVATE Threads::Threads benchmark::benchmark benchmark::benchmark_main)