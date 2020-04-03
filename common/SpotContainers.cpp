//
// Created by Oz Zafar on 03/04/2020.
//

#include "SpotContainers.h"
#include <iostream>

SpotContainers::SpotContainers(int startFloor, int numOfFlours) : startFloor(startFloor), numOfFlours(numOfFlours) {}

// region methods

void SpotContainers::load(Container& container) {
    if (howManyAvailiable() > 1) {
        containers.push_back(container);
        incrementNumOfActions();
    }
}

void SpotContainers::unload(string& containerId){
    int upperContainers = 0;
    bool found = false;
    list<Container> :: reverse_iterator rit;
    for (rit=containers.rbegin(); rit!=containers.rend(); ++rit){
        if (rit->getId().compare(containerId) == 0){
            advance(rit, 1);
            containers.erase(rit.base());
            // have to unload this target container and also load & unload all its upper containers
            setNumOfActions(getNumOfActions()+2*upperContainers+1);
            found = true;
            break;
        }
        upperContainers++;
    }
    if (!found){
        std::cout << "Container with id: " + containerId + " wasn't found" << std::endl;
    }
}

int SpotContainers::howManyAvailiable() {
    return (numOfFlours-startFloor+1)-containers.size();
}

void SpotContainers::incrementNumOfActions() {
    numOfActions+=1;
}

void SpotContainers::decrementNumOfActions() {
    numOfActions-=1;
}

SpotContainers::~SpotContainers() {
}

// endregion

// region Getters&Setter

int SpotContainers::getNumOfActions() const {
    return numOfActions;
}

void SpotContainers::setNumOfActions(int numOfActions) {
    SpotContainers::numOfActions = numOfActions;
}

// endregion
