//
// Created by Oz Zafar on 03/04/2020.
//

#ifndef CPP_STOWAGE_CONTAINERSPOSITION_H
#define CPP_STOWAGE_CONTAINERSPOSITION_H

#include "Container.h"
#include <list>
#include <string>

using std::list;
using std::string;

class ContainersPosition{

private:
    int numOfFloors;
    int startFloor = 0;
    list<string> containers;

public:

    ContainersPosition(int startFloor, int numOfFlours);

    ContainersPosition(int numOfFlours);

    int load(const string& containerId);

    int unload(const string& containerId);

    int howManyAvailiable();

    void setStartFloor(int startFloor);

    void setNumOfFloors(int numOfFloors);

    int getStartFloor() const;

    int getNumOfActiveFloors() const;

    int getTopFloorNumber();

    virtual ~ContainersPosition();

    list<string, std::allocator<string>>::iterator begin();

    list<string>::iterator end();

    string& getTop();

    // string& pop();
};


#endif //CPP_STOWAGE_CONTAINERSPOSITION_H
