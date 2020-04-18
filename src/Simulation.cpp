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
    string path, fileName, content, writingPath, algorithmName;
    size_t i, totalOperations = 0;
    Route* shipRoute;
    Ship* ship;
    CraneManagement::CraneManagementAnswer craneManagementAnswer;

    vector <string> containersAwaitingAtPortInputFiles;
    std::map<string,int> indexOfVisitAtPort;
    std::map<string,int> totalNumbersOfVisitingPort;
    std::map<string,int> indexOfFirstContainersAwaitingAtPortInputFile;

    for(auto& travel: std::filesystem::directory_iterator(rootPath))
    {

        path = travel.path().string();
        std::cout << "----------------" <<"Starting simulate " << path << "----------------" <<std::endl;

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

        for(auto& algorithm: algorithms)
        {
            std::cout <<"===========================================================" << std::endl;
            std::cout <<"starting simulate " << algorithm->getName() << " on " << path << std::endl;
            std::cout <<"===========================================================" << std::endl;
            CraneManagement craneManagement(path + "/_simulation.errors.csv");
            WeightBalanceCalculator weightBalanceCalculator;
            algorithm->setCalculator(weightBalanceCalculator);

            writingPath = path + "/_simulation.results.csv";
            content = algorithm->getName() + ",";
            writeToFile(fout, writingPath, content);

            writingPath = path + "/_simulation.errors.csv";
            content = algorithm->getName() + ",";
            writeToFile(fout, writingPath, algorithm->getName());

            std::cout <<"Reading route..." << std::endl;
            algorithm->readShipRoute(path + "/route.txt");
            shipRoute = IO::readShipRoute(path + "/route.txt");
            vector<string> route = shipRoute->getPorts();

            std::cout <<"Reading ship plan..." << std::endl;
            ship = new Ship();
            algorithm->readShipPlan(path + "/ship_plan.txt");
            IO::readShipPlan(path + "/ship_plan.txt", ship);

            std::cout <<"Updating indexOfFirstContainersAwaitingAtPortInputFile..." << std::endl;
            for(i=0 ; i<containersAwaitingAtPortInputFiles.size() ; i++)
            {
                if(indexOfFirstContainersAwaitingAtPortInputFile.find(containersAwaitingAtPortInputFiles[i].substr(0, 5))==indexOfFirstContainersAwaitingAtPortInputFile.end())
                {
                    indexOfFirstContainersAwaitingAtPortInputFile.insert(std::pair<string, int>(containersAwaitingAtPortInputFiles[i].substr(0, 5),i));
                }
            }

            for (i = 0; i < route.size(); i++)
            {
                std::cout << "----------------" <<"Ship Arrived to port " << route[i] << "----------------" << std::endl;
                if(indexOfVisitAtPort[route[i]]+1 > totalNumbersOfVisitingPort[route[i]])
                {
                    std::cout << "No containers awaiting at port input file for " << route[i] << " for visiting number " << indexOfVisitAtPort[route[i]] + 1 << std::endl;
                    getInstructionsForCargoFromAlgorithm(algorithm, ship,route[i], "", path + "/" + route[i] + "_" + algorithm->getName() + "_instructions_" + std::to_string(indexOfVisitAtPort[route[i]]) + ".txt");
                }
                else
                {
                    getInstructionsForCargoFromAlgorithm(algorithm, ship, route[i], path + "/" + containersAwaitingAtPortInputFiles[indexOfFirstContainersAwaitingAtPortInputFile[route[i]]+indexOfVisitAtPort[route[i]]] + ".txt",path + "/" + route[i] + "_" + algorithm->getName() + "_instructions_" + std::to_string(indexOfVisitAtPort[route[i]]) + ".txt");
                }

                craneManagementAnswer = craneManagement.readAndExecuteInstructions(ship, path + "/" + route[i] + "_" + algorithm->getName() + "_instructions_" + std::to_string(indexOfVisitAtPort[route[i]]) + ".txt");

                totalOperations += craneManagementAnswer.numOfOperations;
                fout.open(path + "/_simulation.errors.csv", std::fstream::app);
                checkForErrorsAfterPort(ship, route[i], fout, craneManagementAnswer, shipRoute);
                fout.close();
                indexOfVisitAtPort[route[i]]++;
                std::cout << "----------------" <<"Ship left port " << route[i] << "----------------" << std::endl;
                shipRoute->incrementCurrentPort();
            }

            writingPath = path + "/_simulation.results.csv";
            content = std::to_string(totalOperations) + ",";
            writeToFile(fout, writingPath, content);

            totalOperations = 0;

            writingPath = path + "/_simulation.errors.csv";
            content = "\n";
            writeToFile(fout, writingPath, content);

            writingPath = path + "/_simulation.results.csv";
            content = "\n";
            writeToFile(fout, writingPath, content);

            delete ship;
            delete shipRoute;
            indexOfVisitAtPort.clear();

            algorithmName = algorithm->getName();
            delete algorithm;
            algorithm = Algorithm::createAlgorithmByName(algorithmName);

        }
        containersAwaitingAtPortInputFiles.clear();
        totalNumbersOfVisitingPort.clear();
        indexOfFirstContainersAwaitingAtPortInputFile.clear();


    }
}

void Simulation::writeToFile(std::ofstream &fout,string& path, string content)  {
    fout.open(path, std::fstream::app);
    if(fout.is_open())
    {
        fout << content;
    }
    fout.close();
}

void Simulation::getInstructionsForCargoFromAlgorithm(Algorithm* algorithm,Ship* ship, const string& port, const string &input_path, const string &output_path) {
    algorithm->getInstructionsForCargo(port, input_path, output_path);
    IO::readContainerAwaitingAtPortFile(input_path,ship);
}

void Simulation::checkForErrorsAfterPort(Ship* ship, const string &port, std::ofstream &fout, CraneManagement::CraneManagementAnswer& answer,Route* route) {
    if (fout.is_open()){
        if (ship->portToContainers[port].size() > 0){
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



