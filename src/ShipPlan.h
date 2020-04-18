//
// Created by Oz Zafar on 03/04/2020.
//

#ifndef CPP_STOWAGE_SHIPPLAN_H
#define CPP_STOWAGE_SHIPPLAN_H

#include <vector>
#include <map>
#include <string>
#include "ContainersPosition.h"
#include <iostream>

using std::vector;
using std::map;

class ShipPlan {
    vector<vector<ContainersPosition>> plan;

public:
    ShipPlan();

    ShipPlan(const vector<vector<ContainersPosition>> &plan);

    void setStartFloorInPosition(int x, int y, int startFloor);

    ContainersPosition& getContainerPosition(int x, int y);

    void printPlan();

    int getPlanLength();

    int getPlanWidth();

};



#endif //CPP_STOWAGE_SHIPPLAN_H
