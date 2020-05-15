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
    int floors;
public:
    ShipPlan();

    void setStartFloorInPosition(int x, int y, int startFloor);

    ContainersPosition& getContainerPosition(int x, int y);

    int getFloors() const;

    void printPlan();

    int getPlanLength();

    int getPlanWidth();

    ShipPlan(const vector<vector<ContainersPosition>> &plan, int floors);
};



#endif //CPP_STOWAGE_SHIPPLAN_H
