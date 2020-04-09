//
// Created by Oz Zafar on 03/04/2020.
//

#include "ShipPlan.h"

ShipPlan::ShipPlan() {
}

ShipPlan::ShipPlan(vector<vector<ContainersPosition>> &plan) : plan(plan) {}

void ShipPlan::load(int row, int column, string& containerId) {
    plan[row][column].load(containerId);
}

void ShipPlan::unload(int row, int column, string &containerId) {
    plan[row][column].unload(containerId);
}

ShipPlan::~ShipPlan() {

}

ShipPlan::ShipPlan(vector<vector<ContainersPosition>> &plan) : plan(plan) {}

