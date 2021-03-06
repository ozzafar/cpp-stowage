//
// Created by Oz Zafar on 03/04/2020.
//

#include "Ship.h"

#include <utility>

Ship::Ship() = default;

void Ship::updateContainerMapping(Container container) {
    this->containerIdToContainer[container.getId()] = container;
}

ShipPlan& Ship::getShipPlan() {
    return shipPlan;
}

string& Ship::containerIdToDestination(string &id) {
    return containerIdToContainer[id].getDestinationPort();
}

int Ship::containerIdToWeight(const string &id) {
    return containerIdToContainer[id].getWeight();
}

Container& Ship::getContainerOfId(const string &id) {
    return containerIdToContainer[id];
}

bool Ship::knowContainerId(const string& id) {
    return containerIdToContainer.count(id) > 0;
}

int Ship::getAmountOfContainers() {
    int amount = 0;
    for (int i = 0 ; i < shipPlan.getPlanLength() ; i++){
        for (int j = 0 ; j < shipPlan.getPlanWidth() ; j ++){
            amount+=shipPlan.getContainerPosition(i,j).getNumOfActiveFloors();
        }
    }
    return amount;
}

