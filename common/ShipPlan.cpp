//
// Created by Oz Zafar on 03/04/2020.
//

#include <iostream>
#include "ShipPlan.h"

ShipPlan::ShipPlan() {
}

ShipPlan::ShipPlan(int numberOfFloors, const vector<vector<ContainersPosition>> &plan) : numberOfFloors(numberOfFloors),
                                                                                         plan(plan) {}

void ShipPlan::setNumOfActiveFloorsInPosition(int x, int y, int numOfActiveFlours) {
    plan[x][y].setNumOfActiveFlours(numOfActiveFlours);
}

void ShipPlan::setStartFloorInPosition(int x, int y, int startFloor) {
    plan[x][y].setStartFloor(startFloor);
}

void ShipPlan::printPlan() {
    int m = plan.size(), n = plan[0].size();
    for (int i = 0 ; i < m ; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << "in position: (" << i << "," << j << ") the start floor is " << plan[i][j].getStartFloor() << std::endl;
        }
    }
}

ContainersPosition& ShipPlan::getContainerPosition(int x, int y) {
    return plan[x][y];
}

void ShipPlan::setPlan(vector<vector<ContainersPosition>> &plan) {
    ShipPlan::plan = plan;
}

const vector<vector<ContainersPosition>> &ShipPlan::getPlan() const {
    return plan;
}

ShipPlan::~ShipPlan() {
}




