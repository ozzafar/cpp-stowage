//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_TRAVEL_H
#define CPP_STOWAGE_TRAVEL_H
#include "ShipPlan.h"
#include "Route.h"


class Travel {
    ShipPlan shipPlan;
    Route route;
    string travelName;

public:
    Travel(ShipPlan shipPlan, Route route, string travelName);
};


#endif //CPP_STOWAGE_TRAVEL_H
