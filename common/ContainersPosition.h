//
// Created by Oz Zafar on 03/04/2020.
//

#ifndef CPP_STOWAGE_CONTAINERSPOSITION_H
#define CPP_STOWAGE_CONTAINERSPOSITION_H

#include "Container.h"
#include <stack>
#include <string>

using std::stack;
using std::string;

class ContainersPosition{

private:
    int startFloor;
    int numOfActiveFloors;
    stack<string> containers;

public:

    ContainersPosition(int startFloor, int numOfFlours);

    ContainersPosition();

    int load(string& containerId);

    int unload(string& containerId);

    int howManyAvailiable();

    void setNumOfActiveFlours(int numOfActiveFlours);

    void setStartFloor(int startFloor);

    int getStartFloor() const;

    int getNumOfActiveFloors() const;

    virtual ~ContainersPosition();

};


#endif //CPP_STOWAGE_CONTAINERSPOSITION_H
