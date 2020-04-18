//
// Created by Oz Zafar on 09/04/2020.
//

#include "Simulation.h"
#include "CraneManagement.h"
#include "../common/IO.h"


Simulation::Simulation(const list<Algorithm*> &algorithms, const string &rootPath) : algorithms(algorithms), rootPath(rootPath) {}

void print(std::list<std::string> const &list)
{
    for (auto const& i: list) {
        std::cout << i << "\n";
    }
}

void Simulation::runSimulation() {
    std::cout <<"Starting simulation" << std::endl;
    string path, fileName;
    size_t i, totalOperations = 0;
    Route *shipRoute;
    Ship* ship = new Ship();
    CraneManagement::CraneManagementAnswer craneManagementAnswer;

    vector <string> containersAwaitingAtPortInputFiles;
    std::map<string,int> indexOfVisitAtPort;
    std::map<string,int> totalNumbersOfVisitingPort;
    std::map<string,int> indexOfFirstContainersAwaitingAtPortInputFile;

    for(auto& travel: std::filesystem::directory_iterator(rootPath)){

        path = travel.path().string();
        std::cout <<"Starting simulate " << path << std::endl;

        std::cout <<"update containersAwaitingAtPortInputFiles and totalNumbersOfVisitingPort" << std::endl;
        for(auto& file: std::filesystem::directory_iterator(path))
        {
            fileName = file.path().stem().string();
            if (fileName.substr(5, 1) == "_") {
                if (isdigit(fileName.substr(6, 1).front()) != 0) {
                    containersAwaitingAtPortInputFiles.push_back(fileName);
                    totalNumbersOfVisitingPort[fileName.substr(0, 5)]++;
                }
            }
        }

        sort(containersAwaitingAtPortInputFiles.begin(), containersAwaitingAtPortInputFiles.end());

        std::cout <<"finish update containersAwaitingAtPortInputFiles and totalNumbersOfVisitingPort" << std::endl;

        std::ofstream fout;
        fout.open(path + "/simulation.results.csv", std::fstream::app);

        for(Algorithm *algorithm: algorithms)
        {
            CraneManagement craneManagement(path + "/simulation.errors.csv");
            std::cout <<"starting simulate " << algorithm->getName() << " on " << path << std::endl;

            if(fout.is_open())
            {
                fout << algorithm->getName() << ",";
            }
            fout.close();

            std::cout <<"Reading route..." << std::endl;
            algorithm->readShipRoute(path + "/route.txt");
            shipRoute = IO::readShipRoute(path + "/route.txt");
            vector<string> route = shipRoute->getPorts();
            std::cout <<"Reading ship plan..." << std::endl;
            algorithm->readShipPlan(path + "/ship_plan.txt");
            IO::readShipPlan(path + "/ship_plan.txt",ship);



            std::cout <<"Updating indexOfFirstContainersAwaitingAtPortInputFile..." << std::endl;

            for(i=0 ; i<containersAwaitingAtPortInputFiles.size() ; i++)
            {
                if(indexOfFirstContainersAwaitingAtPortInputFile.find(containersAwaitingAtPortInputFiles[i].substr(0, 5))==indexOfFirstContainersAwaitingAtPortInputFile.end())
                {
                    indexOfFirstContainersAwaitingAtPortInputFile.insert(std::pair<string, int>(containersAwaitingAtPortInputFiles[i].substr(0, 5),i));
                }
            }

            std::cout <<"Finished updating indexOfFirstContainersAwaitingAtPortInputFile..." << std::endl;

            for (i = 0; i < route.size(); i++) {
                std::cout <<"Ship Arrived to port " << route[i] << std::endl;
                if(indexOfVisitAtPort[route[i]]+1 > totalNumbersOfVisitingPort[route[i]])
                {
                    std::cout << "No containers awaiting at port input file for " << route[i] << " for visiting number " << indexOfVisitAtPort[route[i]] + 1 << std::endl;
                    getInstructionsForCargoFromAlgorithm(algorithm, ship,route[i], "", path + "/" + route[i] + "_instructions_" + std::to_string(indexOfVisitAtPort[route[i]]) + ".txt");
                }
                else
                {
                    getInstructionsForCargoFromAlgorithm(algorithm, ship, route[i], path + "/" + containersAwaitingAtPortInputFiles[indexOfFirstContainersAwaitingAtPortInputFile[route[i]]+indexOfVisitAtPort[route[i]]] + ".txt",path + "/" + route[i] + "_instructions_" + std::to_string(indexOfVisitAtPort[route[i]]) + ".txt");
                }
                craneManagementAnswer = craneManagement.readAndExecuteInstructions(ship, path + "/" + route[i] + "_instructions_" + std::to_string(indexOfVisitAtPort[route[i]]) + ".txt");
                fout.open(path + "/simulation.results.csv", std::fstream::app);
                totalOperations += craneManagementAnswer.numOfOperations;
                checkForErrorsAfterPort(ship, route[i], fout, craneManagementAnswer, shipRoute);
                indexOfVisitAtPort[route[i]]++;
                std::cout <<"Ship left port " << route[i] << std::endl;
                shipRoute->incrementCurrentPort();
            }
            if(fout.is_open())
            {
                fout << totalOperations << ",";
            }
            totalOperations = 0;
        }
        if(fout.is_open())
        {
            fout << std::endl;
        }
    }
}

void Simulation::getInstructionsForCargoFromAlgorithm(Algorithm* algorithm,Ship* ship, const string& port, const string &input_path, const string &output_path) {
    algorithm->getInstructionsForCargo(port, input_path, output_path);
    IO::readContainerAwaitingAtPortFile(input_path,ship);
}

void Simulation::checkForErrorsAfterPort(Ship* ship, const string &port, std::ofstream &fout, CraneManagement::CraneManagementAnswer& answer,Route* route) {
    if (fout.is_open()){
        if (ship->portToContainers.count(port)>0){
            fout << "Not all of the containers of this port were unloaded,";
        }
        if (answer.changedContainers.count(CraneOperation::REJECT) > 0) {
            for (auto& rejected : answer.changedContainers[CraneOperation::REJECT]) {
                for (auto &loaded : answer.changedContainers[CraneOperation::LOAD]) {
                    if (route->nextStopForPort(ship->containerIdToDestination(rejected)) <
                        route->nextStopForPort(ship->containerIdToDestination(loaded))) {
                        fout << "Rejected container " << rejected << " with higher priority,";
                    }
                }
            }
        }
        if (route->inLastStop()) {
            int amountOfContainers = ship->getAmountOfContainers() > 0;
            if (amountOfContainers>0){
                fout << "Ship arrived to last port in route but there are still " << amountOfContainers << " containers on the ship,";
            }
        }
    }
}



