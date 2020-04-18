//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_NAIVEALGORITHM_H
#define CPP_STOWAGE_NAIVEALGORITHM_H

#include "ShipPlan.h"
#include "WeightBalanceCalculator.h"
#include "Container.h"
#include "Route.h"
#include "CraneOperation.h"
#include "Algorithm.h"
#include <string>
#include <map>
#include "ContainersPosition.h"
#include <algorithm>

using std::map;
using std::string;

class NaiveAlgorithm : public Algorithm {

public:

    void getInstructionsForCargo(const string& port, const string &input_path, const string &output_path) override;

    virtual string getName() override;

    virtual ~NaiveAlgorithm();

    void sortContainers(vector<Container *>& containers);

    /* find the lowest position in port which
     * contains a container of specific destination*/
    int findLowestPlaceOfPortInPosition(const string &port, ContainersPosition &position); // private

    void getLoadInstructions(const string &input_path, const string &output_path); // private

    void getUnloadInstructions(const string& port, const string &output_path); // private
};


#endif //CPP_STOWAGE_NAIVEALGORITHM_H
