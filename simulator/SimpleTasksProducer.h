//
// Created by Oz Zafar on 06/06/2020.
//

#ifndef CPP_STOWAGE_SIMPLETASKSPRODUCER_H
#define CPP_STOWAGE_SIMPLETASKSPRODUCER_H


#include <atomic>
#include <mutex>
#include <optional>
#include <thread>
#include <iostream>
#include <functional>
#include "NumTasks.h"
#include "../common/objects/Travel.h"
#include "../interfaces/AbstractAlgorithm.h"
#include "Simulation.h"

using std::vector;

class SimpleTasksProducer {

    const int numTasks = -1;
    std::atomic_int task_counter = 0;
    std::mutex m;
    std::optional<int> next_task_index();
    std::optional<int> next_task_index_simple();
    vector<Travel>& travels;
    vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> factoryVec;
    Simulation& simulation;


public:
    SimpleTasksProducer(NumTasks numTasks, vector<Travel> &travels, vector<std::function<std::unique_ptr<AbstractAlgorithm>()>> &factoryVec, Simulation &simulation);
    SimpleTasksProducer(SimpleTasksProducer &&other);
    std::optional<std::function<void(void)>> getTask();
};



#endif //CPP_STOWAGE_SIMPLETASKSPRODUCER_H
