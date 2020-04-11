//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_ALGORITHM_H
#define CPP_STOWAGE_ALGORITHM_H

#include "ShipPlan.h"
#include "WeightBalanceCalculator.h"
#include "Container.h"
#include "Route.h"
#include "CraneOperation.h"
#include <string>
#include <map>

using std::map;
using std::string;

class Algorithm {
private:
    ShipPlan *shipPlan;
    Route shipRoute;
    WeightBalanceCalculator calculator;

    map<string, Container *> containerIdToContainer;

public:

    void readShipPlan(const string &path);

    void readShipRoute(const string &path);

    void getInstructionsForCargo(const string &input_path, const string &output_path);

    vector<Container *> readContainerAwaitingAtPortFile(const string &path);

    ShipPlan *getShipPlan(); // for testing

    void updateContainerMapping(vector<Container*> containers);

private:
    void breakLineToWords(string &line, vector<string> &row);
};



#endif //CPP_STOWAGE_ALGORITHM_H
