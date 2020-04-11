//
// Created by Oz Zafar on 03/04/2020.
//

#ifndef CPP_STOWAGE_SHIP_H
#define CPP_STOWAGE_SHIP_H

#include "ShipPlan.h"

class Ship {
private:
    ShipPlan shipPlan;
public:
    Ship();

    void load(int row, int column, string& containerId);

    void unload(int row, int column, string& containerId);

};




#endif //CPP_STOWAGE_SHIP_H
