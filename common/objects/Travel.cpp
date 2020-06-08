//
// Created by Oz Zafar on 09/04/2020.
//

#include "Travel.h"


Travel::Travel(ShipPlan shipPlan, Route route, string travelName, string travelPath) : shipPlan(shipPlan) , route(route), travelName(travelName), travelPath(travelPath){
}

const string &Travel::getTravelName() const {
    return travelName;
}

const ShipPlan &Travel::getShipPlan() const {
    return shipPlan;
}

const Route &Travel::getRoute() const {
    return route;
}

const string &Travel::getTravelPath() const {
    return travelPath;
}
