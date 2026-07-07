#include "threadpool.hpp"
#include "constants.hpp"

ThreadPool::ThreadPool() :
    thread_muts {std::vector<std::mutex>(N_THREADS)}
    , cvs {std::vector<std::condition_variable>(N_THREADS)} {

    auto work = [&] (int ind) {
        while (1) {
            std::unique_lock l{ thread_muts[ind] };
            cvs[ind].wait(l);
            thread_states[ind] = false;
            tasks[ind]();
            thread_states[ind] = true;
        }
    };

    for (int i = 0; i < N_THREADS; ++i) {
        tasks.push_back([](){});
        thread_states.push_back(true);
        thread_pool.push_back(std::thread { work, i });
    }

}

// TODO: Notify when threads are available instead of continuous looping
void ThreadPool::queue_work(std::function<void()> func) {
    bool completed = false;
    do {
        for (int i = 0; i < N_THREADS; ++i) {
            std::lock_guard l(thread_muts[i]);
            if (thread_states[i]) {
                tasks[i] = func;
                cvs[i].notify_one();
                completed = true;
                break;
            }
        } 
    } while (!completed);
}
