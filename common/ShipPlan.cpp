//
// Created by Oz Zafar on 03/04/2020.
//

#include "ShipPlan.h"

ShipPlan::ShipPlan(string path) {
    // read input from file
    // --------- template ---------
    for (int i = 0; i < 10; ++i) {
        int row;
        int column;
        int startFloor;
        plan[row][column] = SpotContainers(startFloor,0);
    }
    // --------- template ---------
}

void ShipPlan::load(int row, int column, Container &container) {
    plan[row][column].load(container);
}

void ShipPlan::unload(int row, int column, string &containerId) {
    plan[row][column].unload(containerId);
}

ShipPlan::~ShipPlan() {

}

