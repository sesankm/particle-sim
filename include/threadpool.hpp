#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <functional>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <thread>

class ThreadPool {
private:
    std::vector<bool> thread_states;
    std::vector<std::mutex> thread_muts;
    std::vector<std::condition_variable> cvs;
    std::vector<std::thread> thread_pool;
    std::vector<std::function<void()>> tasks;

public:
    ThreadPool();
    void queue_work(std::function<void()> func);
};

#endif
