#include "threadpool.hpp"
#include "constants.hpp"
#include <thread>

ThreadPool::ThreadPool() :
    thread_muts {std::vector<std::mutex>(N_THREADS)}
    , cvs {std::vector<std::condition_variable>(N_THREADS)} {

    auto work = [&] (int ind) {
        while (1) {
            std::unique_lock l{ thread_muts[ind] };
            cvs[ind].wait(l, [&]{ return thread_states[ind] == false; });
            tasks[ind]();
            thread_states[ind] = true;
            active_threads--;
            active_threads.notify_one();
        }
    };

    for (int i = 0; i < N_THREADS; ++i) {
        tasks.push_back([](){});
        thread_states.push_back(true);
        thread_pool.push_back(std::thread { work, i });
    }
}

void ThreadPool::queue_work(std::function<void()> func, int ind) {
    std::lock_guard l(thread_muts[ind]);
    active_threads++;
    tasks[ind] = func;
    thread_states[ind] = false;
    cvs[ind].notify_one();
}

void ThreadPool::wait() {
    int cur = active_threads.load();
    while (cur != 0) {
        active_threads.wait(cur);
        cur = active_threads.load();
    }
}
