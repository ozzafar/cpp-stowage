//
// Created by Oz Zafar on 03/04/2020.
//

#include "ContainersPosition.h"
#include <iostream>

ContainersPosition::ContainersPosition(int startFloor, int numOfFlours) : startFloor(startFloor), numOfActiveFloors(numOfFlours) {}

ContainersPosition::ContainersPosition() {}


// region methods

//TODO after calling load - add container to the ship containers-mapping
int ContainersPosition::load(string & containerId) {
    if (howManyAvailiable() > 0) {
        containers.push(containerId);
        std::cout << "Container with id: " + containerId + " was loaded" << std::endl;
        return SUCCESS;
    }
    std::cout << "No available place for container with id: " + containerId << std::endl;
    return FAILURE;
}

//TODO after calling load - remove container to the ship containers-mapping
int ContainersPosition::unload(string& containerId){
    string& topId = containers.top();
    if (topId.compare(containerId) == 0){
        containers.pop();
        std::cout << "Container with id: " + containerId + " was unloaded" << std::endl;
        return SUCCESS;
    } else {
        std::cout << "Container with id: " + containerId + " wasn't found" << std::endl;
        return FAILURE;
    }
}

int ContainersPosition::howManyAvailiable() {
    return (numOfActiveFloors - startFloor + 1) - containers.size();
}

void ContainersPosition::setNumOfActiveFlours(int numOfActiveFlours) {
    ContainersPosition::numOfActiveFloors = numOfActiveFlours;
}

void ContainersPosition::setStartFloor(int startFloor) {
    ContainersPosition::startFloor = startFloor;
}

int ContainersPosition::getStartFloor() const {
    return startFloor;
}

int ContainersPosition::getNumOfActiveFloors() const {
    return numOfActiveFloors;
}

ContainersPosition::~ContainersPosition() {
}


// endregion
