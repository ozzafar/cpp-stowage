//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_ALGORITHM_H
#define CPP_STOWAGE_ALGORITHM_H

#include "ShipPlan.h"
#include "WeightBalanceCalculator.h"
#include "Container.h"
#include "Route.h"
#include <string>
#include <map>

using std::map;
using std::string;

class Algorithm {
private:
    ShipPlan shipPlan;
    Route shipRoute;
    WeightBalanceCalculator calculator;

    map<string,Container> containerIdToContainer;

public:

    Algorithm();

    void readShipPlan(const string& path);
    void readShipRoute(const string& path);

    ShipPlan &getShipPlan();
};



#endif //CPP_STOWAGE_ALGORITHM_H
