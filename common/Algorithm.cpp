//
// Created by Oz Zafar on 09/04/2020.
//

#include "Algorithm.h"
#include <fstream>
#include "ContainersPosition.h"

Algorithm::Algorithm() {}

void Algorithm::readShipPlan(const string &path) {
    int numOfFloors, X, Y;
    std::ifstream planFile;
    planFile.open(path);
    planFile >> numOfFloors >> X >> Y;
    shipPlan.setNumberOfFloors(numOfFloors);

    int x, y, actualNumOfFloors;
    while (planFile >> x >> y >> actualNumOfFloors){
        shipPlan.setStartFloorInPosition(x,y,numOfFloors-actualNumOfFloors-1);
    }
}

void Algorithm::readShipRoute(const string &path) {

}

ShipPlan &Algorithm::getShipPlan() {
    return shipPlan;
}
