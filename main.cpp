#include <iostream>
#include "common/Container.h"
#include "common/NaiveAlgorithm.h"
#include "common/Simulation.h"
#include <list>
using std::list;



int main() {

    NaiveAlgorithm *naiveAlgorithm = new NaiveAlgorithm();
    list<Algorithm*> algorithms;
    algorithms.push_back(naiveAlgorithm);
    Simulation simulation(algorithms, "C:/input");
    simulation.RunSimulation();
    return 0;
}
