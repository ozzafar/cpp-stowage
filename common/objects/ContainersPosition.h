//
// Created by Oz Zafar on 03/04/2020.
//

#ifndef CPP_STOWAGE_CONTAINERSPOSITION_H
#define CPP_STOWAGE_CONTAINERSPOSITION_H

#include "Container.h"
#include <list>
#include <string>
#include <iostream>

using std::list;
using std::string;

class ContainersPosition{

private:
    int startFloor = 0;
    int numOfFloors;
    list<string> containers;

public:

    ContainersPosition(int numOfFloors);

    int load(const string& containerId, bool print);

    int load(const string& containerId);

    int unload(const string& containerId, bool print);

    int unload(const string& containerId);

    static int move(ContainersPosition &source, ContainersPosition &target, const string& containerId);

    // return how many places are available for loading containers
    int howManyAvailiable();

    void setStartFloor(int startFloorP);

    int getStartFloor() const;

    // how many places are occupied by containers
    int getNumOfActiveFloors() const;

    // return number of the top floor which has a container
    int getTopFloorNumber();

    list<string, std::allocator<string>>::iterator begin();

    list<string>::iterator end();

    string& getTop();

};


#endif //CPP_STOWAGE_CONTAINERSPOSITION_H
