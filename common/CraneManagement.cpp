//
// Created by Oz Zafar on 11/04/2020.
//

#include "CraneManagement.h"

int CraneManagement::load(ShipPlan& shipPlan, int row, int column, string& containerId) {
    ContainersPosition position = shipPlan.getContainerPosition(row, column);
    if (position.load(containerId)){
        position.setNumOfActiveFlours(position.getNumOfActiveFloors()+1);
        return SUCCESS;
    } else{
        return FAILURE;
    }
}

int CraneManagement::unload(ShipPlan& shipPlan,int row, int column, string &containerId) {
    ContainersPosition position = shipPlan.getContainerPosition(row, column);
    if (shipPlan.getContainerPosition(row, column).unload(containerId)){
        position.setNumOfActiveFlours(position.getNumOfActiveFloors()-1);
        return SUCCESS;
    } else {
        return FAILURE;
    }
}

int CraneManagement::move(ShipPlan &shipPlan, int oldRow, int oldColumn, int newRow, int newColumn, string &containerId) {
    int unload = shipPlan.getContainerPosition(oldRow, oldColumn).unload(containerId);
    if (unload){
        return shipPlan.getContainerPosition(newRow, newColumn).load(containerId);
    }
    return FAILURE;
}





