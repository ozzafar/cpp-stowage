#include "src/NaiveAlgorithm.h"
#include "src/Simulation.h"
#include <list>

using std::list;


int main() {
    string root = "C://input/";

    NaiveAlgorithm *naiveAlgorithm = new NaiveAlgorithm();
    list<Algorithm*> algorithms;
    algorithms.push_back(naiveAlgorithm);
    Simulation simulation(algorithms, root);
    simulation.runSimulation();

    for (Algorithm* algorithm : algorithms){
        delete algorithm;
    }

    return 0;
}
