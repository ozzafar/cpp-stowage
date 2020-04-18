#include "src/NaiveAlgorithm.h"
#include "src/Simulation.h"
#include <list>

using std::list;


int main() {
    string root = "/Users/ozzafar/CLionProjects/cpp-stowage/tests/input/";

    NaiveAlgorithm *naiveAlgorithm = new NaiveAlgorithm();
    list<Algorithm*> algorithms;
    algorithms.push_back(naiveAlgorithm);
    Simulation simulation(algorithms, root);
    simulation.runSimulation();
    return 0;
}
