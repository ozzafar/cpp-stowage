//
// Created by Oz Zafar on 03/04/2020.
//

#ifndef CPP_STOWAGE_SHIPPLAN_H
#define CPP_STOWAGE_SHIPPLAN_H

#include <vector>
#include <string>
#include "SpotContainers.h"

using std::vector;

class ShipPlan {

    vector<vector<SpotContainers>> plan;

public:

    ShipPlan(const string path);

    void load(int row, int column, Container& container);

    void unload(int row, int column, string& containerId);

    virtual ~ShipPlan();

};



#endif //CPP_STOWAGE_SHIPPLAN_H
