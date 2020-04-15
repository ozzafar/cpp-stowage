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

using std::map;
using std::string;

class NaiveAlgorithm : public Algorithm {

public:

    void getInstructionsForCargo(const string& port, const string &input_path, const string &output_path) override;

    virtual string* getName() override;


// private: (in comment for tests)

    bool operator()(Container *a, Container *b) {
        return portToIndexesInRoute[a->getDestinationPort()][0] <= portToIndexesInRoute[b->getDestinationPort()][0]; // private
    }

    void sortContainers(vector<Container *>& containers);

    int findLowestPlaceOfPortInPosition(const string &containerId, ContainersPosition &position); // private

    void getLoadInstructions(const string &input_path, const string &output_path); // private

    void getUnloadInstructions(const string& port, const string &output_path); // private
};


#endif //CPP_STOWAGE_NAIVEALGORITHM_H
