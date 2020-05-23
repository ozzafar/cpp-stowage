//
// Created by Oz Zafar on 11/04/2020.
//

//
// Created by Oz Zafar on 10/04/2020.
//

#include "gtest/gtest.h"
#include "../common/objects/Container.cpp"
#include "../algorithm/NaiveAlgorithm.cpp"
#include "../algorithm/NaiveAlgorithmHorizontal.cpp"
#include "../algorithm/NaiveAlgorithmVertical.cpp"
#include "../algorithm/BasicAlgorithm.cpp"
#include "../common/objects/Route.cpp"
#include "../common/objects/ContainersPosition.cpp"
#include "../common/objects/Travel.cpp"
#include "../common/objects/ShipPlan.cpp"
#include "../common/objects/Ship.cpp"
#include "../simulation/Simulation.cpp"
#include "../simulation/AlgorithmResults.cpp"
#include "../interfaces/WeightBalanceCalculator.cpp"
#include "../common/objects/CraneManagement.cpp"
#include "../common/utils/IO.cpp"
#include "../common/utils/ErrorsIterator.cpp"
#include "../common/utils/Errors.cpp"
#include "../common/utils/Registrar.cpp"

string prefix = "/Users/ozzafar/CLionProjects/cpp-stowage/tests/";

// region Container tests

TEST(Container, IdValidity){
    Container container(0,"dest","CSQU3054383");
    EXPECT_TRUE(Container::isValidId(container.getId()));
}

//endregion


// region Read Input tests

TEST(ReadShipPlan, printCreatedPlan){
    NaiveAlgorithmVertical algorithm;
    algorithm.readShipPlan(prefix+"plan.csv");
    algorithm.getShip().getShipPlan().printPlan();
}

TEST(ReadShipPlan, printWarning){
    NaiveAlgorithmVertical algorithm;
    algorithm.readShipPlan(prefix+"/bad-plan.csv");
    algorithm.getShip().getShipPlan().printPlan();
}

TEST(ContainerAwaitingAtPortFile, print){
    NaiveAlgorithmVertical algorithm;
    vector<Container> containers;
     algorithm.readContainerAwaitingAtPortFile(prefix+"SSSSS.cargo_data",containers);
    for (Container& container : containers){
        std::cout << container << std::endl;
    }
}

// endregion

// region Containers Position tests

//TEST(ContainersPosition, basicFunctionality){
//    ContainersPosition position(5);
//    EXPECT_EQ(0,position.getNumOfActiveFloors());
//    EXPECT_EQ(3,position.howManyAvailiable());
//    position.load("containerId1", true);
//    EXPECT_EQ("containerId1",position.getTop());
//    EXPECT_EQ(1,position.getNumOfActiveFloors());
//    EXPECT_EQ(2,position.getTopFloorNumber());
//    EXPECT_EQ(2,position.howManyAvailiable());
//    position.load("containerId2", true);
//    EXPECT_EQ("containerId2",position.getTop());
//    EXPECT_EQ(2,position.getNumOfActiveFloors());
//    EXPECT_EQ(3,position.getTopFloorNumber());
//    EXPECT_EQ(1,position.howManyAvailiable());
//    position.unload("containerId1", true);
//    EXPECT_EQ("containerId2",position.getTop());
//    EXPECT_EQ(2,position.getNumOfActiveFloors());
//    EXPECT_EQ(3,position.getTopFloorNumber());
//    EXPECT_EQ(1,position.howManyAvailiable());
//    position.unload("containerId2", true);
//    EXPECT_EQ("containerId1",position.getTop());
//    EXPECT_EQ(1,position.getNumOfActiveFloors());
//    EXPECT_EQ(2,position.getTopFloorNumber());
//    EXPECT_EQ(2,position.howManyAvailiable());
//
//    ContainersPosition position2(3);
//    EXPECT_EQ(1,position2.howManyAvailiable());
//    position2.load("containerId", true);
//    EXPECT_EQ(0,position2.howManyAvailiable());
//}

// endregion

// region Algorithms tests

//TEST(NaiveAlgorithm, sortPortsInRoute) {
//    NaiveAlgorithmVertical algorithm;
//    algorithm.readShipRoute(prefix+"route.csv");
//    vector<Container> containers;
//    algorithm.readContainerAwaitingAtPortFile(prefix+"SSSSS.cargo_data",containers);
//    algorithm.sortContainers(containers);
//    std::cout<<"finish";
//}

TEST(NaiveAlgorithm, getLoadInstructionsANDgetUnloadInstructions){
//    NaiveAlgorithmVertical algorithm;
//    algorithm.readShipPlan(prefix+"plan2.csv");
//    algorithm.getShip().getShipPlan().printPlan();
//    algorithm.readShipRoute(prefix+"route.csv");
//    algorithm.getLoadInstructions(prefix+"SSSSS.cargo_data",prefix+"NaiveAlgorithm-test1.csv");
//    algorithm.getUnloadInstructions("DDDDD",prefix+"NaiveAlgorithm-test1.csv");
//    algorithm.getUnloadInstructions("AAAAA",prefix+"NaiveAlgorithm-test1.csv");
//    algorithm.getUnloadInstructions("BBBBB",prefix+"NaiveAlgorithm-test1.csv");
//    algorithm.getUnloadInstructions("CCCCC",prefix+"NaiveAlgorithm-test1.csv");
//    algorithm.getUnloadInstructions("EEEEE",prefix+"NaiveAlgorithm-test1.csv");
//    algorithm.getUnloadInstructions("FFFFF",prefix+"NaiveAlgorithm-test1.csv");
//    algorithm.getUnloadInstructions("GGGGG",prefix+"NaiveAlgorithm-test1.csv");
//    EXPECT_EQ(0,algorithm.getShip()->getAmountOfContainers());
}

// endregion

// region Crane Management tests
TEST(CraneManagement, readAndExecuteInstructions) {
    std::ofstream fout;
    string filename = prefix+"simulator.errors.csv";
    fout.open(filename,std::fstream::app);
    if (fout.is_open()) {
        fout << "NaiveAlgorithm,";
    }
    fout.close();

    NaiveAlgorithmVertical algorithm;
    algorithm.readShipPlan(prefix+"plan.csv");
    Ship ship = algorithm.getShip();
    CraneManagement craneManagement;
    craneManagement.readAndExecuteInstructions(ship, prefix + "NaiveAlgorithm-test1.csv");
}
// endregion

TEST(Errors, addError) {
    Errors errors;
    errors.addError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT);
    errors.addError(Error::PORT_APPEAR_TWICE_WARNING);
    EXPECT_TRUE(errors.hasError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT));
    EXPECT_TRUE(errors.hasError(Error::PORT_APPEAR_TWICE_WARNING));
}

TEST(Errors, addErrors) {
    Errors errors1,errors2;
    errors1.addError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT);
    errors1.addError(Error::PORT_APPEAR_TWICE_WARNING);
    errors2.addErrors(errors1);
    EXPECT_TRUE(errors2.hasError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT));
    EXPECT_TRUE(errors2.hasError(Error::PORT_APPEAR_TWICE_WARNING));
}
TEST(Errors, iterator) {
    Errors errors;
    errors.addError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT);
    errors.addError(Error::PORT_APPEAR_TWICE_WARNING);
    ErrorsIterator it = errors.getIterator();
    EXPECT_EQ(it.getNext(),Error::PORT_APPEAR_TWICE_WARNING);
    EXPECT_EQ(it.getNext(),Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT);
}


//Errors errors;
//errors.addError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT);
//errors.addError(Error::PORT_APPEAR_TWICE_WARNING);
//if (!errors.hasError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT)){
//std::cout<< "error1";
//}
//if (!errors.hasError(Error::PORT_APPEAR_TWICE_WARNING)){
//std::cout<< "error2";
//}
//
//
//Errors errors1, errors2;
//errors1.addError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT);
//errors1.addError(Error::PORT_APPEAR_TWICE_WARNING);
//errors2.addErrors(errors1);
//if (!errors.hasError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT)){
//std::cout<< "error3";
//}
//if (!errors.hasError(Error::PORT_APPEAR_TWICE_WARNING)){
//std::cout<< "error4";
//}
//
//Errors errors3;
//errors3.addError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT);
//errors3.addError(Error::PORT_APPEAR_TWICE_WARNING);
//ErrorsIterator it = errors3.getIterator();
//
//if (it.getNext()!=Error::PORT_APPEAR_TWICE_WARNING){
//std::cout<< "error6";
//}
//if (it.getNext()!=Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT){
//std::cout<< "error5";
//}





//Errors errors;
//errors.addError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT);
//errors.addError(Error::PORT_APPEAR_TWICE_WARNING);
//if (!errors.hasError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT)){
//std::cout<< "error1";
//}
//if (!errors.hasError(Error::PORT_APPEAR_TWICE_WARNING)){
//std::cout<< "error2";
//}
//
//
//Errors errors1, errors2;
//errors1.addError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT);
//errors1.addError(Error::PORT_APPEAR_TWICE_WARNING);
//errors2.addErrors(errors1);
//if (!errors.hasError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT)){
//std::cout<< "error3";
//}
//if (!errors.hasError(Error::PORT_APPEAR_TWICE_WARNING)){
//std::cout<< "error4";
//}
//
//Errors errors3;
//errors3.addError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT);
//errors3.addError(Error::PORT_APPEAR_TWICE_WARNING);
//ErrorsIterator it = errors3.getIterator();
//
//if (it.getNext()!=Error::PORT_APPEAR_TWICE_WARNING){
//std::cout<< "error6";
//}
//if (it.getNext()!=Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT){
//std::cout<< "error5";
//}