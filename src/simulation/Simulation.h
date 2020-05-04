//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_SIMULATION_H
#define CPP_STOWAGE_SIMULATION_H

#define ROOT /tmp

#include <list>
#include <filesystem>
#include "../algorithm/AbstractAlgorithm.h"
#include "../algorithm/NaiveAlgorithm.h"
#include "../algorithm/CraneManagement.h"

class Simulation {

private:

    list<AbstractAlgorithm*> algorithms;

    string rootPath;

public:
    Simulation(const list<AbstractAlgorithm*> &algorithms, const string &rootPath);

    void runSimulation();

    void getInstructionsForCargoFromAlgorithm(AbstractAlgorithm* algorithm, Ship& ship, vector<Container>& waitingContainers, const string &input_path, const string &output_path);

    void checkForErrorsAfterPort(Ship& ship, const string &port, std::ofstream &fout,
                                 CraneManagement::CraneManagementAnswer& answer, Route& route);

    void writeToFile(std::ofstream &fout, string& path,  string content);
};


#endif //CPP_STOWAGE_SIMULATION_H
