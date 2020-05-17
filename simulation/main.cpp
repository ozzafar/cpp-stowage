#include "Simulation.h"
#include "../common/utils/Registrar.h"
#include <list>

using std::list;


int main() {
    std::cout<< "started"<<std::endl;
    std::string root = "../algorithm/";
    Simulation simulation("/Users/ozzafar/CLionProjects/cpp-stowage/tests/input");
    simulation.simulateAllTravelsWithAllAlgorithms();
    return 0;
}
