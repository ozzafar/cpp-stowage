//
// Created by Oz Zafar on 03/04/2020.
//

#ifndef CPP_STOWAGE_SHIPPLAN_H
#define CPP_STOWAGE_SHIPPLAN_H

#include <vector>
#include <string>
#include "ContainersPosition.h"

using std::vector;

class ShipPlan {

    vector<vector<ContainersPosition>> plan;

public:
    ShipPlan();

    ShipPlan(vector<vector<ContainersPosition>> &plan);

    ShipPlan(const vector<vector<ContainersPosition>> &plan);

    void load(int row, int column, string& containerId);

    void unload(int row, int column, string& containerId);

    virtual ~ShipPlan();

};



#endif //CPP_STOWAGE_SHIPPLAN_H
