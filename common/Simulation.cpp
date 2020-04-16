//
// Created by Oz Zafar on 09/04/2020.
//

#include "Simulation.h"
#include "NaiveAlgorithm.h"
#include <regex>


Simulation::Simulation(const list<Algorithm*> &algorithms, const string &rootPath) : algorithms(algorithms), rootPath(rootPath) {}

void print(std::list<std::string> const &list)
{
    for (auto const& i: list) {
        std::cout << i << "\n";
    }
}

void Simulation::RunSimulation() {
    string path, fileName;
    Route *shipRoute;
    Ship ship;
    list <string> containersAwaitingAtPortInputFiles;
    std::map<string,int> indexOfVisitAtPort;

    for(auto& p: std::filesystem::directory_iterator(rootPath)){
        path = p.path().string();
        std::cout <<path << std::endl;

        for(auto& p: std::filesystem::directory_iterator(path))
        {
            fileName = p.path().stem().string();
            if (fileName.substr(5, 1) == "_") {
                if (isdigit(fileName.substr(6, 1).front()) != 0) {
                    containersAwaitingAtPortInputFiles.push_back(fileName);
                }
            }
        }

        containersAwaitingAtPortInputFiles.sort();
        containersAwaitingAtPortInputFiles.unique();

        std::cout <<"------------------" <<std::endl;
        print(containersAwaitingAtPortInputFiles);
        std::cout <<"------------------"<< std::endl;

        for(Algorithm *algorithm: algorithms)

        {
            /*algorithm->readShipPlan(path + "/ship_plan.txt");*/
            algorithm->readShipPlan(path + "\\ship_plan.txt");
            algorithm->readShipRoute(path + "\\route.txt");
            shipRoute = algorithm->getShipRoute();
            vector <string> route = shipRoute->getPorts();


            for(int i = 0; i< route.size() ; i++) {
                indexOfVisitAtPort.insert(std::pair<string, int>(route[i],0));
            }




        }
    }

}

Simulation::~Simulation() {

}
