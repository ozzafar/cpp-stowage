//
// Created by Oz Zafar on 11/04/2020.
//

#ifndef CPP_STOWAGE_CRANEMANAGEMENT_H
#define CPP_STOWAGE_CRANEMANAGEMENT_H

#include <string>
#include "ShipPlan.h"

using std::string;

class CraneManagement {
    void load(ShipPlan& plan, int row, int column, string& containerId);

    void unload(ShipPlan& plan,int row, int column, string& containerId);
};


#endif //CPP_STOWAGE_CRANEMANAGEMENT_H
