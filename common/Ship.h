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

    std::map<string, Container *> containerIdToContainer;

public:

    std::map<string, std::set<string>> portToContainers;

    Ship();

    Container& getContainerOfId(const string& id);

    const string& containerIdToDestination(const string& id);

    int containerIdToWeight(const string& id);

    ShipPlan& getShipPlan();

    void setShipPlan(ShipPlan plan);

    bool knowContainerId(const string &id);

    void updateContainerMapping(Container *container);

    int getAmountOfContainers();

    void setContainerIdToContainerMap(map<string, Container *> &containerIdToContainer);

    map<string, Container *> &getContainerIdToContainer();
};




#endif //CPP_STOWAGE_SHIP_H
