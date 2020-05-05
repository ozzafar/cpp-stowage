//
// Created by Yarden on 03/05/2020.
//

#ifndef SIMULATOR_SIMULATION_H
#define SIMULATOR_SIMULATION_H

#include <filesystem>
#include <fstream>
#include "../../common/interfaces/AbstractAlgorithm.h"
#include "../../src/algorithm/NaiveAlgorithm.h"

class Simulation {

private:
    string travelsPath;
    string outputPath;
    list<AbstractAlgorithm*> algorithms;

    void simulateOneTravelWithAllAlgorithms(const string &travelPath);

public:

    Simulation(const string &travelsPath, const string &algorithmPath, const string &outputPath);
    int simulateAllTravelsWithAllAlgorithms();
};

#endif //SIMULATOR_SIMULATION_H
