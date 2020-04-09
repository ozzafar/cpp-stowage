//
// Created by Oz Zafar on 03/04/2020.
//

#ifndef CPP_STOWAGE_CONTAINERSPOSITION_H
#define CPP_STOWAGE_CONTAINERSPOSITION_H

#include "Container.h"
#include <stack>
#include <string>

using std::stack;
using std::string ;

class ContainersPosition{

private:

    stack<string> containers;
    int startFloor;
    int numOfActiveFlours;

public:

    ContainersPosition(int startFloor, int numOfFlours);

    int load(string& containerId);

    int unload(string& containerId);

    int howManyAvailiable();

    virtual ~ContainersPosition();

};


#endif //CPP_STOWAGE_CONTAINERSPOSITION_H
