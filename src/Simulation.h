//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_SIMULATION_H
#define CPP_STOWAGE_SIMULATION_H

#define ROOT /tmp

#include "Algorithm.h"
#include <list>
#include <filesystem>
#include "NaiveAlgorithm.h"
#include "CraneManagement.h"

class Simulation {
private:
    list<Algorithm*> algorithms;
    string rootPath;

public:
    Simulation(const list<Algorithm*> &algorithms, const string &rootPath);

    void runSimulation();

    void getInstructionsForCargoFromAlgorithm(Algorithm* algorithm,Ship* ship, const string& port, const string &input_path, const string &output_path);

    void checkForErrorsAfterPort(Ship* ship, const string &port, std::ofstream &fout,
                                 CraneManagement::CraneManagementAnswer& answer, Route* route);

    void writeToFile(std::ofstream &fout, string& path,  string content);
};


#endif //CPP_STOWAGE_SIMULATION_H
