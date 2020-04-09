//
// Created by Oz Zafar on 09/04/2020.
//

#include "Algorithm.h"
#include "ShipPlan.h"
#include <fstream>
#include "ContainersPosition.h"


void Algorithm::readShipPlan(const string &path) {
    int numOfFloors, X, Y;
    std::ifstream planFile(path);
    planFile >> numOfFloors >> X >> Y;
    shipPlan.setNumberOfFloors(numOfFloors);

    int x, y, actualNumOfFloors;
    while (planFile >> x >> y >> actualNumOfFloors){
        shipPlan.setStartFloorInPosition(x,y,numOfFloors-actualNumOfFloors-1);
    }
}

void Algorithm::readShipRoute(const string &path) {

}