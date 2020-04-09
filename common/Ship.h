//
// Created by Oz Zafar on 03/04/2020.
//

#ifndef CPP_STOWAGE_SHIP_H
#define CPP_STOWAGE_SHIP_H

#include "ShipPlan.h"
#include "Container.h"
#include <map>

using std::map;


class Ship {
private:
    ShipPlan shipPlan;
    map<string,Container> my_map;
public:
    Ship();
};




#endif //CPP_STOWAGE_SHIP_H
