//
// Created by Oz Zafar on 11/04/2020.
//

#ifndef CPP_STOWAGE_CRANEMANAGEMENT_H
#define CPP_STOWAGE_CRANEMANAGEMENT_H

#include <string>
#include "ShipPlan.h"

using std::string;

class CraneManagement {

public:

    // return value is SUCCESS / FAILURE:
    int load(ShipPlan& plan, string& containerId, int floor, int row, int column);

    int unload(ShipPlan& plan, string& containerId, int floor, int row, int column);

    int move(ShipPlan &shipPlan,string &containerId, int oldFloor, int oldRow, int oldColumn, int newRow, int newColumn, int newFloor);

    void readAndExecuteInstructions(ShipPlan &shipPlan, const string &input_path);

};


#endif //CPP_STOWAGE_CRANEMANAGEMENT_H
