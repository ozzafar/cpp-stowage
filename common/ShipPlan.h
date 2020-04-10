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
    int numberOfFloors;
    vector<vector<ContainersPosition>> plan;

public:
    ShipPlan();

    ShipPlan(int numberOfFloors, const vector<vector<ContainersPosition>> &plan);

    void load(int row, int column, string& containerId);

    void unload(int row, int column, string& containerId);

    void setNumberOfFloors(int numberOfFloors);

    void setStartFloorInPosition(int x, int y, int startFloor);

    void setNumOfActiveFloorsInPosition(int x, int y, int numOfActiveFlours);

    void setPlan(vector<vector<ContainersPosition>> &plan);

    void printPlan();

    virtual ~ShipPlan();

};



#endif //CPP_STOWAGE_SHIPPLAN_H
