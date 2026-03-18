#include <iostream>
#include <vector>
#include <stack>
#include <coroutine>
#include <cstdint>
#include <exception>
#include <concepts>
#include <utility>

// https://en.cppreference.com/w/cpp/language/coroutines.html#:~:text=5%206%207-,co_yield,-co_yield%20expression%20returns
template<typename T>
struct Generator {
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type {
        T value_;
        std::exception_ptr exception_;

        Generator get_return_object() {
            return Generator(handle_type::from_promise(*this));
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() { exception_ = std::current_exception(); } 

        template<std::convertible_to<T> From>
        std::suspend_always yield_value(From&& from) {
            value_ = std::forward<From>(from); 
            return {};
        }
        void return_void() {}
    };

    handle_type h_;

    Generator(handle_type h) : h_(h) {}
    ~Generator() { h_.destroy(); }
    
    explicit operator bool() {
        fill(); 
        return !h_.done();
    }
    
    T operator()() {
        fill();
        full_ = false; 
        return std::move(h_.promise().value_);
    }

private:
    bool full_ = false;

    void fill() {
        if (!full_) {
            h_();
            if (h_.promise().exception_)
                std::rethrow_exception(h_.promise().exception_);
            full_ = true;
        }
    }
};

Generator<int> dfs(int start_node, const std::vector<std::vector<int>>& graph) {
    std::vector<bool> visited(graph.size(), false);
    std::stack<int> stack;

    stack.push(start_node);

    while (!stack.empty()) {
        int current_node = stack.top(); 
        stack.pop();
        
        if (visited[current_node] == false) {
            visited[current_node] = true;
            
            std::cout << "[DFS] Yielding node: " << current_node << "\n";
            co_yield current_node; 
            std::cout << "[DFS] Resumed\n";
            
            for (int neighbor : graph[current_node]) {
                if (visited[neighbor] == false) {
                    stack.push(neighbor);
                }
            }
        }
    }
}

int main() {
    std::vector<std::vector<int>> graph = {
        {1, 2},
        {0, 3, 4},
        {0},
        {1},
        {1}
    };

    std::cout << "[Main] Starting cooperative DFS\n";
    auto gen = dfs(0, graph);

    while (gen) {
        std::cout << "[Main] Requesting next node\n";
        int val = gen();
        std::cout << "[Main] Received node: " << val << "\n---\n";
    }

    std::cout << "[Main] DFS completed\n";
    return 0;
}