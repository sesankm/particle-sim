#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <thread>

class ThreadPool {
private:
    // Use a char instead of bools here to avoid bit-level data races
    std::vector<char> thread_states;
    std::vector<std::mutex> thread_muts;
    std::vector<std::condition_variable> cvs;
    std::vector<std::thread> thread_pool;
    std::vector<std::function<void()>> tasks;

public:
    std::mutex mut;
    std::atomic<int> active_threads {0};
    ThreadPool();
    void queue_work(std::function<void()> func, int ind);
    void wait();
};

#endif
