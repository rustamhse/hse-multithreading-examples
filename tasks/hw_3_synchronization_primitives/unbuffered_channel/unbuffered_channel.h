#pragma once

#include <optional>
#include <mutex>
#include <condition_variable>
#include <stdexcept>

template <class T>
class UnbufferedChannel {
public:
    void Send(const T& value) {
        std::unique_lock<std::mutex> lock(mutex_);

        cv_send_.wait(lock, [this]() { 
            return !is_busy_ || is_closed_; 
        });

        if (is_closed_) {
            throw std::runtime_error("Channel is closed");
        }

        is_busy_ = true;
        data_ = value;

        cv_recv_.notify_one();

        cv_ack_.wait(lock, [this]() { 
            return !data_.has_value() || is_closed_;
        });

        if (is_closed_ && data_.has_value()) {
            data_.reset();
            is_busy_ = false;
            throw std::runtime_error("Channel is closed and value was not received");
        }

        is_busy_ = false;
        cv_send_.notify_one(); 
    }

    std::optional<T> Recv() {
        std::unique_lock<std::mutex> lock(mutex_);
        
        cv_recv_.wait(lock, [this]() {
            return is_closed_ || data_.has_value();
        });

        if (!data_.has_value() && is_closed_) {
            return std::nullopt;
        }

        T result = std::move(data_.value());
        data_.reset();

        cv_ack_.notify_one();

        return result;
    }

    void Close() {
        std::lock_guard<std::mutex> lock(mutex_);
        is_closed_ = true;
        cv_send_.notify_all();
        cv_recv_.notify_all();
        cv_ack_.notify_all(); 
    }

private:
    std::optional<T> data_;
    std::mutex mutex_;
    
    bool is_closed_ = false;
    bool is_busy_ = false;
    
    std::condition_variable cv_send_;
    std::condition_variable cv_recv_;
    std::condition_variable cv_ack_;
};