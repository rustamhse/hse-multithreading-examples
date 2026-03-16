#include <vector>
#include <thread>
#include <functional>
#include <algorithm>

#include <iostream>

template <typename T>
void ApplyFunction(std::vector<T>& data, const std::function<void(T&)>& transform, const int threadCount = 1) {
    
    // Ничего не делаем если data пустой
    if (data.size() == 0) {
        return;
    }
    
    // Если число потоков превышает число элементов, то число потоков следует взять равным числу элементов (c)
    int actualThreadCount = std::min(threadCount, static_cast<int>(data.size())); // 
    // Делим data между потоками
    int blockSize = data.size() / actualThreadCount;

    std::vector<std::thread> threads;

    // Раздаем каждому потоку кусок, последний получает остаток (если он есть)
    for (int i = 0; i < actualThreadCount; ++i) {
        int start = i * blockSize;
        int end;

        if (i == (actualThreadCount - 1)){
            end = data.size();
        } else {
            end = (i + 1) * blockSize;
        }

        // Создаем потоки
        threads.emplace_back([&data, &transform, start, end] () {
            for (int i = start; i < end; ++i){
                transform(data[i]);
            }
        });
    }

    // Ожидаем выполнения потоков
    for (int i = 0; i < threads.size(); ++i){
        threads[i].join();
    }
}