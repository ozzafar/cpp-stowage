//
// Created by Oz Zafar on 13/04/2020.
//

#ifndef CPP_STOWAGE_ALGORITHM_H
#define CPP_STOWAGE_ALGORITHM_H

#include "ShipPlan.h"
#include "WeightBalanceCalculator.h"
#include "Container.h"
#include "Route.h"
#include "CraneOperation.h"
#include "Ship.h"
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include "ContainersPosition.h"
#include "CraneOperation.h"
#include <list>
#include <string>
#include <algorithm>

using std::map;
using std::string;


class Algorithm {
public:
    virtual ~Algorithm() = 0;

protected:

    Route *shipRoute;

    WeightBalanceCalculator calculator;

    Ship *ship = new Ship();

    map<string, vector<int>> portToIndexesInRoute;

public:

    virtual void getInstructionsForCargo(const string& port, const string &input_path, const string &output_path) = 0;

    virtual string* getName() = 0;

    void readShipPlan(const string &path);

    void readShipRoute(const string &path);

    vector<Container *> readContainerAwaitingAtPortFile(const string &path);

    void writeOperation(const std::string &filename, CraneOperation op, const string &containerId, int floor, int x, int y);

    Ship *getShip() const;

    Route *getShipRoute() const;

    string &createPositionFromRowInput(int numOfFloors, int X, int Y, string &line) const;
};

#endif //CPP_STOWAGE_ALGORITHM_H
