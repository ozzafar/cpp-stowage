#include "Simulation.h"
#include "../common/utils/Registrar.h"
#include <list>

using std::list;


int main() {
    Simulation simulation("C://input");
    simulation.simulateAllTravelsWithAllAlgorithms();
    return 0;
}
