//
// Created by Oz Zafar on 11/04/2020.
//

//
// Created by Oz Zafar on 10/04/2020.
//

#include "gtest/gtest.h"
#include "../common/Container.cpp"
#include "../common/Algorithm.cpp"
#include "../common/Route.cpp"
#include "../common/ContainersPosition.cpp"
#include "../common/Travel.cpp"
#include "../common/ShipPlan.cpp"
#include "../common/Ship.cpp"
#include "../common/Simulation.cpp"
#include "../common/WeightBalanceCalculator.cpp"
#include "../common/CraneManagement.cpp"
#include "../common/CraneOperation.h"

TEST(ReadShipPlan, printCreatedPlan){
    Algorithm algorithm;
    algorithm.readShipPlan("/Users/ozzafar/CLionProjects/cpp-stowage/tests/plan.csv");
    algorithm.getShipPlan()->printPlan();
    //EXPECT_EQ(0,0);
}

TEST(ReadShipPlan, printWarning){
    Algorithm algorithm;
    algorithm.readShipPlan("/Users/ozzafar/CLionProjects/cpp-stowage/tests/bad-plan.csv");
    algorithm.getShipPlan()->printPlan();
    //EXPECT_EQ(0,0);
}

TEST(ContainerAwaitingAtPortFile, print){
    Algorithm algorithm;
    const vector<Container*> containers = algorithm.readContainerAwaitingAtPortFile(
            "/Users/ozzafar/CLionProjects/cpp-stowage/tests/ILASH_1.cargo_data");
    for (Container* container : containers){
        std::cout << *container << std::endl;
    }
}