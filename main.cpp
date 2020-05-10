#include "src/algorithm/NaiveAlgorithm.h"
#include "src/simulation/Simulation.h"
#include "common/interfaces/AbstractAlgorithm.h"
#include <list>



int main() {

    Simulation simulation("C://input");
    simulation.simulateAllTravelsWithAllAlgorithms();

    return 0;
}
