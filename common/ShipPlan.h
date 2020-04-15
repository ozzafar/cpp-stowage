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

    void setStartFloorInPosition(int x, int y, int startFloor);

    void setNumOfFloorsInPosition(int x, int y, int numOfFloors);

    void setPlan(vector<vector<ContainersPosition>> &plan);

    ContainersPosition& getContainerPosition(int x, int y);

    const vector<vector<ContainersPosition>> &getPlan() const;

    void printPlan();

    int getPlanLength();

    int getPlanWidth();

    virtual ~ShipPlan();

};



#endif //CPP_STOWAGE_SHIPPLAN_H
