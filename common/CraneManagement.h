//
// Created by Oz Zafar on 11/04/2020.
//

#ifndef CPP_STOWAGE_CRANEMANAGEMENT_H
#define CPP_STOWAGE_CRANEMANAGEMENT_H

#include <string>
#include "ShipPlan.h"

using std::string;

class CraneManagement {

    // return value is SUCCESS / FAILURE:

    int load(ShipPlan& plan, int row, int column, string& containerId);

    int unload(ShipPlan& plan,int row, int column, string& containerId);

    int move(ShipPlan& shipPlan,int oldRow, int oldColumn, int newRow, int newColumn, string& containerId);

};


#endif //CPP_STOWAGE_CRANEMANAGEMENT_H
