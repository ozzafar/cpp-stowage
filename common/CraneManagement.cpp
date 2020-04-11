//
// Created by Oz Zafar on 11/04/2020.
//

#include "CraneManagement.h"

void CraneManagement::load(ShipPlan& shipPlan, int row, int column, string& containerId) {
    shipPlan.getContainerPosition(row, column).load(containerId);
}

void CraneManagement::unload(ShipPlan& shipPlan,int row, int column, string &containerId) {
    shipPlan.getContainerPosition(row, column).unload(containerId);
}



