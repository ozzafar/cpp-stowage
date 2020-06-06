//
// Created by Oz Zafar on 06/06/2020.
//

#ifndef CPP_STOWAGE_THREADPOOLEXECUTER_H
#define CPP_STOWAGE_THREADPOOLEXECUTER_H

#include <vector>
#include <atomic>
#include <thread>
#include <iostream>
#include "NumThreads.h"

template<typename Producer>
class ThreadPoolExecuter {
    Producer producer;
    const int numThreads = -1;
    std::vector<std::thread> workers;
    std::atomic_bool running = false;
    std::atomic_bool stopped = false;
    static thread_local int num_tasks_finished;
    std::atomic_int total_num_tasks_finished{0};

    void worker_function() {
        while (!stopped) {
            auto task = producer.getTask();
            if (!task) break;
            (*task)();
            ++num_tasks_finished;
            ++total_num_tasks_finished;
        }
        std::cout << std::this_thread::get_id() << " - finished after processing " << num_tasks_finished << " task(s)"<< std::endl;
    }

public:
    ThreadPoolExecuter(Producer producer, NumThreads numThreads)
            : producer(std::move(producer)), numThreads(numThreads) {
        workers.reserve(numThreads);
    }

    bool start() {
        bool running_status = false;
        // see: https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange
        if (!running.compare_exchange_strong(running_status, true)) {
            return false;
        }
        for (int i = 0; i < numThreads; ++i) {
            workers.push_back(std::thread([this] {
                worker_function();
            }));
        }
        return true;
    }

    void wait_till_finish() {
        for (auto &t : workers) {
            t.join();
        }
        std::cout << "thread pool finished/stopped after processing " << total_num_tasks_finished << " task(s)"
                  << std::endl;
    }
};

template<typename Producer>
thread_local int ThreadPoolExecuter<Producer>::num_tasks_finished{0};

#endif //CPP_STOWAGE_THREADPOOLEXECUTER_H
