//
// Created by Oz Zafar on 03/04/2020.
//

#include "ShipPlan.h"

ShipPlan::ShipPlan() {
}

void ShipPlan::load(int row, int column, string& containerId) {
    plan[row][column].load(containerId);
}

void ShipPlan::unload(int row, int column, string &containerId) {
    plan[row][column].unload(containerId);
}

ShipPlan::~ShipPlan() {

}

void ShipPlan::setNumberOfFloors(int numberOfFloors) {
    ShipPlan::numberOfFloors = numberOfFloors;
}

void ShipPlan::setNumOfActiveFloorsInPosition(int x, int y, int numOfActiveFlours) {
    plan[x][y].setNumOfActiveFlours(numOfActiveFlours);
}

void ShipPlan::setStartFloorInPosition(int x, int y, int startFloor) {
    plan[x][y].setStartFloor(startFloor);
}

