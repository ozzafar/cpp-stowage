//
// Created by Oz Zafar on 09/04/2020.
//

#include "Simulation.h"
#include "CraneManagement.h"


Simulation::Simulation(const list<Algorithm*> &algorithms, const string &rootPath) : algorithms(algorithms), rootPath(rootPath) {}

void print(std::list<std::string> const &list)
{
    for (auto const& i: list) {
        std::cout << i << "\n";
    }
}



void Simulation::RunSimulation() {
    std::cout <<"Starting simulation" << std::endl;
    string path, fileName;
    int i, totalOperations = 0, index = 0;
    Route *shipRoute;
    Ship ship;
    CraneManagement::CraneManagementAnswer craneManagementAnswer;

    vector <string> containersAwaitingAtPortInputFiles;
    std::map<string,int> indexOfVisitAtPort;
    std::map<string,int> totalNumbersOfVisitingPort;
    std::map<string,int> indexOfFirstContainersAwaitingAtPortInputFile;



    for(auto& p: std::filesystem::directory_iterator(rootPath)){

        path = p.path().string();
        std::cout <<"Starting simulate " << path << std::endl;
        CraneManagement craneManagement(path + "/simulation.errors.csv");


        std::cout <<"update containersAwaitingAtPortInputFiles and totalNumbersOfVisitingPort" << std::endl;
        for(auto& p: std::filesystem::directory_iterator(path))
        {
            fileName = p.path().stem().string();
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
            std::cout <<"starting simulate " << algorithm->getName() << " on " << path << std::endl;

            if(fout.is_open())
            {
                fout << algorithm->getName() << ",";
                fout.close();
            }

            std::cout <<"Reading ship plan..." << std::endl;
            algorithm->readShipPlan(path + "\\ship_plan.txt");
            std::cout <<"Reading route..." << std::endl;
            algorithm->readShipRoute(path + "\\route.txt");
            shipRoute = algorithm->getShipRoute();
            vector <string> route = shipRoute->getPorts();
            ship = *(algorithm->getShip());


            std::cout <<"Updating indexOfFirstContainersAwaitingAtPortInputFile..." << std::endl;

            for(i=0 ; i<containersAwaitingAtPortInputFiles.size() ; i++)
            {
                if(indexOfFirstContainersAwaitingAtPortInputFile.find(containersAwaitingAtPortInputFiles[i].substr(0, 5))==indexOfFirstContainersAwaitingAtPortInputFile.end())
                {
                    indexOfFirstContainersAwaitingAtPortInputFile.insert(std::pair<string, int>(containersAwaitingAtPortInputFiles[i].substr(0, 5),i));
                }
            }

            std::cout <<"Finished pdating indexOfFirstContainersAwaitingAtPortInputFile..." << std::endl;

            for(i = 0; i< route.size() ; i++) {

                std::cout <<"Ship Arrived to port " << route[i] << std::endl;
                if(indexOfVisitAtPort[route[i]]+1 > totalNumbersOfVisitingPort[route[i]])
                {
                    std::cout << "no containers awaiting at port input file for " << route[i] << "for visiting number " << indexOfVisitAtPort[route[i]] << std::endl;
                    getInstructionsForCargoFromAlgorithm(*algorithm, ship,route[i], "", path + "/" + route[i] + "_instructions_" + std::to_string(indexOfVisitAtPort[route[i]]) + ".txt");
                }
                else
                {
                    string a = path + "/" + containersAwaitingAtPortInputFiles[indexOfFirstContainersAwaitingAtPortInputFile[route[i]]+indexOfVisitAtPort[route[i]]]+".txt";
                    getInstructionsForCargoFromAlgorithm(*algorithm, ship, route[i], path + "/" + containersAwaitingAtPortInputFiles[indexOfFirstContainersAwaitingAtPortInputFile[route[i]]+indexOfVisitAtPort[route[i]]] + ".txt",path + "/" + route[i] + "_instructions_" + std::to_string(indexOfVisitAtPort[route[i]]) + ".txt");
                }

                craneManagementAnswer = craneManagement.readAndExecuteInstructions(ship, path + "/" + route[i] + "_instructions_" + std::to_string(indexOfVisitAtPort[route[i]]) + ".txt");
                fout.open(path + "/simulation.results.csv", std::fstream::app);
                totalOperations += craneManagementAnswer.numOfOperations;
                checkForErrorsAfterPort(ship, route[i], fout);
                indexOfVisitAtPort[route[i]]++;
                std::cout <<"Ship left port " << route[i] << std::endl;
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

Simulation::~Simulation() {

}

void Simulation::getInstructionsForCargoFromAlgorithm(Algorithm &algorithm,Ship &ship, const string& port, const string &input_path, const string &output_path) {
    algorithm.getInstructionsForCargo(port, input_path, output_path);
    ship.setContainerIdToContainerMap(algorithm.getShip()->getContainerIdToContainer());

}

void Simulation::checkForErrorsAfterPort(Ship &ship, const string &port, std::ofstream &fout) {


}


