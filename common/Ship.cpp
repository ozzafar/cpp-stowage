//
// Created by Oz Zafar on 03/04/2020.
//

#include "Ship.h"

Ship::Ship() {
}

void Ship::load(int row, int column, string& containerId) {
    shipPlan.getContainerPosition(row, column).load(containerId);
}

void Ship::unload(int row, int column, string &containerId) {
    shipPlan.getContainerPosition(row, column).unload(containerId);
}

