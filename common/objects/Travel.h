//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_TRAVEL_H
#define CPP_STOWAGE_TRAVEL_H
#include "ShipPlan.h"
#include "Route.h"


class Travel {
    ShipPlan shipPlan;
public:
    const string &getTravelPath() const;

private:
    Route route;
    string travelName;
    string travelPath;

public:
    Travel(ShipPlan shipPlan, Route route, string travelName, string travelPath);

    const string &getTravelName() const;

    const ShipPlan &getShipPlan() const;

    const Route &getRoute() const;
};


#endif //CPP_STOWAGE_TRAVEL_H
