cmake_minimum_required(VERSION 3.14)
project(MultiThreadHW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

include_directories(include)

add_executable(unit_tests tests/test_apply_function.cpp)
target_link_libraries(unit_tests gtest_main)

add_executable(run_benchmarks tests/apply_function_benchmark.cpp)

target_link_libraries(run_benchmarks benchmark::benchmark benchmark::benchmark_main)