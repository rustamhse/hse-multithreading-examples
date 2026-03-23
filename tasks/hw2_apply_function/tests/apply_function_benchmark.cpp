#include <benchmark/benchmark.h>
#include "apply_function.h"
#include <vector>
#include <cmath>

auto heavy_task = [](double& n){
    n = std::sin(n) + std::cos(n);
};

static void BM_SingleThread_HeavyWork(benchmark::State& state) {
    size_t size = state.range(0);
    std::vector<double> data(size, 1.0);

    for (auto _ : state) {
        ApplyFunction<double>(data, heavy_task, 1);
    }
}
BENCHMARK(BM_SingleThread_HeavyWork)->Range(10000, 100000);

static void BM_MultiThread_HeavyWork(benchmark::State& state) {
    size_t size = state.range(0);
    std::vector<double> data(size, 1.0);

    for (auto _ : state) {
        ApplyFunction<double>(data, heavy_task, 4); 
    }
}
BENCHMARK(BM_MultiThread_HeavyWork)->Range(10000, 100000);