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
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include "../common/utils/IO.h"
#include "../common/objects/CraneManagement.h"
#include "../common/utils/Registrar.h"
#include "../interfaces/AbstractAlgorithm.h"
#include "../interfaces/WeightBalanceCalculator.h"
#include "AlgorithmResults.h"
#include "../common/objects/Travel.h"

class Simulation {

private:
    string travelsPath;
    vector <string> travelNames;
    vector <Travel> travels;
    string algorithmPath;
    string outputPath;
    bool isOutputPathSupplied = false;
    map<std::string, AlgorithmResults> algorithmsResults;
    int numOfThreads = 1;
    static const string GENERAL_ERRORS;
    static const string ROUTE;
    static const string SHIP_PLAN;
    static const string SIMULATION_RESULTS;

public:

    Errors simulateOneTravelWithOneAlgorithm(Travel travel, std::unique_ptr<AbstractAlgorithm> algorithm, const string &algorithmName);
    Simulation(const string &travelsPath, const string &algorithmPath = "", const string &outputPath = "", int numOfThreads = 1);
    int simulateAllTravelsWithAllAlgorithms();

    int checkForErrorsAfterPort(Ship &ship, const string &port,
                            CraneManagement::CraneManagementAnswer &answer,
                            Route &route, vector<Container>& containers, vector<Container>& badContainers,Errors& errors);

    int checkTravelsPath(const string &travelsPathToCheck);

    bool checkIfAllLoadedContainersAreCloser(Ship &ship, CraneManagement::CraneManagementAnswer &answer, Route &route, Container &container);

    void simulateOneTravelWithAllAlgorithms(Travel travel);
};


#endif //SIMULATOR_SIMULATION_H
