//
// Created by Yarden on 03/05/2020.
//

#ifndef SIMULATOR_SIMULATION_H
#define SIMULATOR_SIMULATION_H

#include <filesystem>
#include <fstream>
#include <string>
#include "../../common/interfaces/AbstractAlgorithm.h"
#include "../../common/utils/IO.h"
#include "../../common/errors/Errors.h"
#include "../../src/algorithm/NaiveAlgorithm.h"
#include "../../src/algorithm/CraneManagement.h"
#include "AlgorithmResults.h"

class Simulation {

private:
    string travelsPath;
    string algorithmPath;
    string outputPath;
    bool isOutputPathSupplied = false;
    list<AbstractAlgorithm*> algorithms;
    map<string, AlgorithmResults> algorithmResults;

    void simulateOneTravelWithAllAlgorithms(const string &travelPath);
    void simulateOneTravelWithOneAlgorithm(const string &travelPath, AbstractAlgorithm *algorithm);

public:

    Simulation(const string &travelsPath, const string &algorithmPath = "", const string &outputPath = "");
    int simulateAllTravelsWithAllAlgorithms();

    void checkForErrorsAfterPort(Ship &ship, const string &port,
                            CraneManagement::CraneManagementAnswer &answer,
                            Route &route);
};

#endif //SIMULATOR_SIMULATION_H
