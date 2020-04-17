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
    string path, fileName;
    int i, index=0;
    Route *shipRoute;
    Ship *ship;
    CraneManagement::CraneManagementAnswer craneManagementAnswer;

    vector <string> containersAwaitingAtPortInputFiles;
    std::map<string,int> indexOfVisitAtPort;
    std::map<string,int> totalNumbersOfVisitingPort;
    std::map<string,int> indexOfFirstContainersAwaitingAtPortInputFile;


    for(auto& p: std::filesystem::directory_iterator(rootPath)){
        path = p.path().string();
        CraneManagement craneManagement(path + "/errors.txt");
        std::cout <<path << std::endl;

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


        for(Algorithm *algorithm: algorithms)
        {
            /*algorithm->readShipPlan(path + "/ship_plan.txt");*/
            algorithm->readShipPlan(path + "\\ship_plan.txt");
            algorithm->readShipRoute(path + "\\route.txt");
            shipRoute = algorithm->getShipRoute();
            vector <string> route = shipRoute->getPorts();
            ship = algorithm->getShip();

            for(i=0 ; i<containersAwaitingAtPortInputFiles.size() ; i++)
            {
                if(indexOfFirstContainersAwaitingAtPortInputFile.find(containersAwaitingAtPortInputFiles[i].substr(0, 5))==indexOfFirstContainersAwaitingAtPortInputFile.end())
                {
                    indexOfFirstContainersAwaitingAtPortInputFile.insert(std::pair<string, int>(containersAwaitingAtPortInputFiles[i].substr(0, 5),i));
                }
            }

            for(i = 0; i< route.size()-1 ; i++) {
                if(indexOfVisitAtPort[route[i]] > totalNumbersOfVisitingPort[route[i]])
                {
                    algorithm->getInstructionsForCargo(route[i], "", path + "/" + route[i] + "_instructions_" + std::to_string(indexOfVisitAtPort[route[i]]) + ".txt");
                }
                else
                {
                    string a = path + "/" + containersAwaitingAtPortInputFiles[indexOfFirstContainersAwaitingAtPortInputFile[route[i]]+indexOfVisitAtPort[route[i]]]+".txt";
                    algorithm->getInstructionsForCargo(route[i], path + "/" + containersAwaitingAtPortInputFiles[indexOfFirstContainersAwaitingAtPortInputFile[route[i]]+indexOfVisitAtPort[route[i]]]+".txt",path + "/" + route[i] + "_instructions_" + std::to_string(indexOfVisitAtPort[route[i]]) + ".txt");
                }

                craneManagementAnswer = craneManagement.readAndExecuteInstructions(&(*ship), path + "/" + route[i] + "_instructions_" + std::to_string(indexOfVisitAtPort[route[i]]) + ".txt");
                indexOfVisitAtPort[route[i]]++;
            }





        }
    }

}

Simulation::~Simulation() {

}
