//
// Created by Oz Zafar on 03/04/2020.
//

#ifndef CPP_STOWAGE_SPOTCONTAINERS_H
#define CPP_STOWAGE_SPOTCONTAINERS_H

#include "Container.h"
#include <list>

using std::list;

class SpotContainers{

private:

    list<Container> containers;
    int numOfActions = 0;
    int startFloor;
    int numOfFlours;

public:

    SpotContainers(int startFloor, int numOfFlours);

    void load(Container& container);

    void unload(string& containerId);

    int howManyAvailiable();

    int getNumOfActions() const;

    void incrementNumOfActions();

    void decrementNumOfActions();

    virtual ~SpotContainers();

    void setNumOfActions(int numOfActions);

};


#endif //CPP_STOWAGE_SPOTCONTAINERS_H
