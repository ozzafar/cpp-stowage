//
// Created by Oz Zafar on 03/04/2020.
//

#ifndef CPP_STOWAGE_SHIP_H
#define CPP_STOWAGE_SHIP_H

#include "ShipPlan.h"

typedef ShipPlan* ShipPlanP;

class Ship {
private:
    ShipPlanP shipPlan = nullptr;
public:
    Ship(string path);
};




#endif //CPP_STOWAGE_SHIP_H
