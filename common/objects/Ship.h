//
// Created by Oz Zafar on 03/04/2020.
//

#ifndef CPP_STOWAGE_SHIP_H
#define CPP_STOWAGE_SHIP_H

#include "ShipPlan.h"
#include <map>
#include <set>

class Ship {

private:

    ShipPlan shipPlan;

public:

    std::map<string, Container> containerIdToContainer;

    std::map<string, std::set<string>> portToContainers;

    Ship(const Ship* ship)
    {
        shipPlan = (*ship).shipPlan, containerIdToContainer = (*ship).containerIdToContainer;
    }

    Ship();

    Container& getContainerOfId(const string& id);

    string& containerIdToDestination(string& id);

    int containerIdToWeight(const string& id);

    ShipPlan& getShipPlan();

    // check if the ship has encountered this container Id
    bool knowContainerId(const string &id);

    void updateContainerMapping(Container container);

    int getAmountOfContainers();

};




#endif //CPP_STOWAGE_SHIP_H
