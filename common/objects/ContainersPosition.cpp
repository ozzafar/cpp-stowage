//
// Created by Oz Zafar on 03/04/2020.
//

#include "ContainersPosition.h"
#include "../utils/Errors.h"


// region methods


int ContainersPosition::load(const string& containerId, bool print) {
    if (howManyAvailiable() > 0) {
        containers.push_back(containerId);
        if(print)
        {
            std::cout << "Container with id: " + containerId + " was loaded" << std::endl;
        }
        return (int)Error::SUCCESS;
    }
    std::cout << "No available place for container with id: " + containerId << std::endl;
    return 1;
}

int ContainersPosition::unload(const string& containerId, bool print){
    string& topId = containers.back();
    if (topId == containerId){
        containers.pop_back();
        if(print)
        {
            std::cout << "Container with id: " + containerId + " was unloaded" << std::endl;
        }
        return (int)Error::SUCCESS;
    } else {
        std::cout << "Container with id: " + containerId + " isn't on top of the position" << std::endl;
        return 1;
    }
}

int ContainersPosition::howManyAvailiable() {
    return numOfFloors - startFloor - containers.size();
}

void ContainersPosition::setStartFloor(int startFloorP) {
    ContainersPosition::startFloor = startFloorP;
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

ContainersPosition::ContainersPosition(int numOfFloors) : numOfFloors(numOfFloors) {}



// endregion
