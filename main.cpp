#include <iostream>
#include "common/Container.h"
#include "common/Algorithm.h"

int main() {
    Container container(0, "port1", "id");
    Algorithm algorithm;
    algorithm.readShipPlan("/Users/ozzafar/CLionProjects/cpp-stowage/tests/test1.csv");
    algorithm.getShipPlan()->printPlan();
    return 0;
}
