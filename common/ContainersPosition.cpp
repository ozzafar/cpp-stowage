//
// Created by Oz Zafar on 03/04/2020.
//

#include "ContainersPosition.h"


ContainersPosition::ContainersPosition(int startFloor, int numOfFloors) : startFloor(startFloor), numOfFloors(numOfFloors) {}

ContainersPosition::ContainersPosition(int numOfFloors) : numOfFloors(numOfFloors) {}


// region methods

int ContainersPosition::load(const string& containerId) {
    if (howManyAvailiable() > 0) {
        containers.push_back(containerId);
        std::cout << "Container with id: " + containerId + " was loaded" << std::endl;
        return SUCCESS;
    }
    std::cout << "No available place for container with id: " + containerId << std::endl;
    return FAILURE;
}

int ContainersPosition::unload(const string& containerId){
    string& topId = containers.back();
    if (topId == containerId){
        containers.pop_back();
        std::cout << "Container with id: " + containerId + " was unloaded" << std::endl;
        return SUCCESS;
    } else {
        std::cout << "Container with id: " + containerId + " isn't on top of the position" << std::endl;
        return FAILURE;
    }
}

int ContainersPosition::howManyAvailiable() {
    return (numOfFloors - startFloor) - containers.size();
}

void ContainersPosition::setStartFloor(int startFloor) {
    ContainersPosition::startFloor = startFloor;
}

int ContainersPosition::getStartFloor() const {
    return startFloor;
}

int ContainersPosition::getNumOfActiveFloors() const {
    return containers.size();
}

int ContainersPosition::getTopFloorNumber() {
    return startFloor+getNumOfActiveFloors()-1;
}

list<string>::iterator ContainersPosition::begin()
{
    return containers.begin();
}

list<string>::iterator ContainersPosition::end()
{
    return containers.end();
}


string &ContainersPosition::getTop() {
    return containers.back();
}

// endregion
