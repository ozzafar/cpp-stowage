#include <iostream>
#include "common/Container.h"
#include "common/Algorithm.h"

int main() {
    Container container(0, "port1", "id");
    Algorithm algorithm;
    algorithm.readShipPlan("/Users/ozzafar/CLionProjects/cpp-stowage/tests/test1.csv");
    algorithm.getShipPlan().printPlan();
    auto id =  container.getId();
    id.at(0)='x';
    std::cout << id << std::endl;
    std::cout << container.getId() << std::endl;
    return 0;
}
