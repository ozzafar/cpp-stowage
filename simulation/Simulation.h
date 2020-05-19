//
// Created by Yarden on 03/05/2020.
//

#ifndef SIMULATOR_SIMULATION_H
#define SIMULATOR_SIMULATION_H

#include <memory>
#include <functional>
#include <filesystem>
#include <fstream>
#include <string>
#include "../interfaces/AbstractAlgorithm.h"
#include "../common/utils/IO.h"
#include "../common/utils/Errors.h"
//#include "../algorithm/NaiveAlgorithm.h"
#include "../common/objects/CraneManagement.h"
#include "AlgorithmResults.h"
#include "../common/utils/Registrar.h"

class Simulation {

private:
    string travelsPath;
    string algorithmPath;
    string outputPath;
    bool isOutputPathSupplied = false;
    map<string, AlgorithmResults> algorithmsResults;

    void simulateOneTravelWithAllAlgorithms(const string &travelPath);
    void simulateOneTravelWithOneAlgorithm(const string &travelPath, unique_ptr<AbstractAlgorithm> algorithm, const string &algorithmName);

public:


    Simulation(const string &travelsPath, const string &algorithmPath = "", const string &outputPath = "");
    int simulateAllTravelsWithAllAlgorithms();

    void checkForErrorsAfterPort(Ship &ship, const string &port,
                            CraneManagement::CraneManagementAnswer &answer,
                            Route &route, const string &pathOfOutputFilesForAlgorithmAndTravel, vector<Container>& containers, vector<Container>& badContainers);

    int checkTravelsPath(const string &travelsPathToCheck);

    bool checkIfAllLoadedContainersAreCloser(Ship &ship, CraneManagement::CraneManagementAnswer &answer, Route &route, Container &container);
};

#endif //SIMULATOR_SIMULATION_H
