#include <gtest/gtest.h>
#include <chrono>
#include <cmath>
#include "apply_function.h"

TEST(ApplyFunctionTest, TestCorrectness) {
    std::vector<int> data = {1, 2, 3};
    std::vector<int> expected = {2, 4, 6};
    ApplyFunction<int>(data, [](int& n) {n *= 2;}, 2);

    EXPECT_EQ(data, expected);
}

TEST(ApplyFunctionTest, TimeMeasurement) {

    const size_t size = 1000000;
    std::vector<double> data(size, 1.0);

    auto heavy_task = [](double& n){
        n = std::sin(n) + std::cos(n);
    };

    // Однопоточная версия
    auto start_time_single_thread = std::chrono::high_resolution_clock::now();
    ApplyFunction<double>(data, heavy_task, 1);
    auto finish_time_single_thread = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration_single_thread =  finish_time_single_thread - start_time_single_thread;

    // Многопоточная версия
    auto start_time_multi_thread = std::chrono::high_resolution_clock::now();
    ApplyFunction<double>(data, heavy_task, 4);
    auto finish_time_multi_thread = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration_multi_thread =  finish_time_multi_thread - start_time_multi_thread;

    bool multi_faster_than_single = duration_multi_thread < duration_single_thread;

    EXPECT_TRUE(multi_faster_than_single);
}

TEST(ApplyFunctionTest, MoreThreadsThanElements) {
    std::vector<int> data = {1, 2, 3};
    std::vector<int> expected = {2, 4, 6};
    ApplyFunction<int>(data, [](int& n) {n *= 2;}, 10);

    EXPECT_EQ(data, expected);
}

TEST(ApplyFunctionTest, HandlesEmptyVector){
    std::vector<int> data = {};
    EXPECT_NO_THROW(ApplyFunction<int>(data, [](int& n) {n++;}, 4));
}