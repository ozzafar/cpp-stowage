//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_SIMULATION_H
#define CPP_STOWAGE_SIMULATION_H

#define ROOT /tmp

#include "Algorithm.h"
#include <list>
#include <filesystem>

class Simulation {
private:
    list<Algorithm*> algorithms;
    string rootPath;

public:

    Simulation(const list<Algorithm*> &algorithms, const string &rootPath);

    virtual ~Simulation();

    void RunSimulation();


};


#endif //CPP_STOWAGE_SIMULATION_H
