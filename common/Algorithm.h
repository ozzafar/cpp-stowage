//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_ALGORITHM_H
#define CPP_STOWAGE_ALGORITHM_H

#include <string>
#include "ShipPlan.h"
#include "Route.h"
#include "WeightBalanceCalculator.h"

using std::string;

class Algorithm {
private:
    ShipPlan shipPlan;
    Route shipRoute;
    WeightBalanceCalculator calculator;
public:
    Algorithm();
    void readShipPlan(const string& path);
    void readShipRoute(const string& path);
};



#endif //CPP_STOWAGE_ALGORITHM_H
