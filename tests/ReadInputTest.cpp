//
// Created by Oz Zafar on 11/04/2020.
//

//
// Created by Oz Zafar on 10/04/2020.
//

#include "gtest/gtest.h"
#include "../common/Container.cpp"
#include "../common/NaiveAlgorithm.cpp"
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


// region Read Input tests
TEST(ReadShipPlan, printCreatedPlan){
    NaiveAlgorithm algorithm;
    algorithm.readShipPlan("/Users/ozzafar/CLionProjects/cpp-stowage/tests/plan.csv");
    algorithm.getShip()->getShipPlan().printPlan();
}

TEST(ReadShipPlan, printWarning){
    NaiveAlgorithm algorithm;
    algorithm.readShipPlan("/Users/ozzafar/CLionProjects/cpp-stowage/tests/bad-plan.csv");
    algorithm.getShip()->getShipPlan().printPlan();
}

TEST(ContainerAwaitingAtPortFile, print){
    NaiveAlgorithm algorithm;
    const vector<Container*> containers = algorithm.readContainerAwaitingAtPortFile(
            "/Users/ozzafar/CLionProjects/cpp-stowage/tests/ILASH_1.cargo_data");
    for (Container* container : containers){
        std::cout << *container << std::endl;
    }
}

// endregion

// region Containers Position tests

TEST(ContainersPosition, basicFunctionality){
    ContainersPosition position(2,5);
    EXPECT_EQ(0,position.getNumOfActiveFloors());
    EXPECT_EQ(4,position.howManyAvailiable());
    position.load("containerId1");
    EXPECT_EQ("containerId1",position.getTop());
    EXPECT_EQ(1,position.getNumOfActiveFloors());
    EXPECT_EQ(2,position.getTopFloorNumber());
    EXPECT_EQ(3,position.howManyAvailiable());
    position.load("containerId2");
    EXPECT_EQ("containerId2",position.getTop());
    EXPECT_EQ(2,position.getNumOfActiveFloors());
    EXPECT_EQ(3,position.getTopFloorNumber());
    EXPECT_EQ(2,position.howManyAvailiable());
    position.unload("containerId1");
    EXPECT_EQ("containerId2",position.getTop());
    EXPECT_EQ(2,position.getNumOfActiveFloors());
    EXPECT_EQ(3,position.getTopFloorNumber());
    EXPECT_EQ(2,position.howManyAvailiable());
    position.unload("containerId2");
    EXPECT_EQ("containerId1",position.getTop());
    EXPECT_EQ(1,position.getNumOfActiveFloors());
    EXPECT_EQ(2,position.getTopFloorNumber());
    EXPECT_EQ(3,position.howManyAvailiable());
}

// endregion

// region Algorithms tests

TEST(NaiveAlgorithm, test1){
    NaiveAlgorithm algorithm;
    algorithm.readShipPlan("/Users/ozzafar/CLionProjects/cpp-stowage/tests/plan.csv");
    algorithm.readShipRoute("/Users/ozzafar/CLionProjects/cpp-stowage/tests/plan.csv");
    algorithm.getLoadInstructions("ILASH_1.cargo_data","NaiveAlgorithm-test1");
}

// endregion
