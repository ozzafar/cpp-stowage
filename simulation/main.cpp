#include "Simulation.h"
#include "../common/utils/Registrar.h"
#include <list>

using std::list;


int main() {
    //std::cout<< "started"<<std::endl;
    //std::string root = "../algorithm/";
    //Registrar::getInstance().loadSO(root+"_206039984_a.so");
    Simulation simulation("C://input");
    simulation.simulateAllTravelsWithAllAlgorithms();

    return 0;
}
