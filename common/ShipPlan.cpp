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
        plan[row][column] = ContainersPosition(startFloor, 0);
    }
    // --------- template ---------
}

void ShipPlan::load(int row, int column, string& containerId) {
    plan[row][column].load(containerId);
}

void ShipPlan::unload(int row, int column, string &containerId) {
    plan[row][column].unload(containerId);
}

ShipPlan::~ShipPlan() {

}

ShipPlan::ShipPlan(vector<vector<ContainersPosition>> &plan) : plan(plan) {}

