//
// Created by Oz Zafar on 09/04/2020.
//

#include "Simulation.h"
#include "NaiveAlgorithm.h"

Simulation::Simulation(const list<Algorithm*> &algorithms, const string &rootPath) : algorithms(algorithms),
                                                                                    rootPath(rootPath) {}
void Simulation::RunSimulation() {
    string path;
    Route *shipRoute;
    Ship ship;

    for(auto& p: std::filesystem::directory_iterator(rootPath)){
        path = p.path().string();
        std::cout <<path << std::endl;
        for(Algorithm *algorithm: algorithms)
        {
            /*algorithm->readShipPlan(path + "/ship_plan.txt");*/
            algorithm->readShipPlan(path + "\\ship_plan.txt");
            algorithm->readShipRoute(path + "\\route.txt");

            shipRoute = algorithm->getShipRoute();

            for(string portId: shipRoute->getPorts())
            {
                algorithm->getInstructionsForCargo("AAAAA", path + "/AAAAA.txt", path + "/AAAAA_instructions.txt");
            }


        }
    }

}

Simulation::~Simulation() {

}
