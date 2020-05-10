//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_NAIVEALGORITHM_H
#define CPP_STOWAGE_NAIVEALGORITHM_H


#include "../common/objects/ShipPlan.h"
#include "WeightBalanceCalculator.h"
#include "../common/objects/Container.h"
#include "../common/objects/Route.h"
#include "../interfaces/AbstractAlgorithm.h"
#include <string>
#include <map>
#include "../common/objects/ContainersPosition.h"
#include "BasicAlgorithm.h"
#include <algorithm>

using std::map;
using std::string;

class NaiveAlgorithm : public BasicAlgorithm {

public:

    NaiveAlgorithm();

    NaiveAlgorithm(const NaiveAlgorithm &naiveAlgorithm) {
        calculator = naiveAlgorithm.calculator, portToIndexesInRoute = naiveAlgorithm.portToIndexesInRoute,

        route = new Route(naiveAlgorithm.route), ship = new Ship(naiveAlgorithm.ship);
    }

    int getInstructionsForCargo(const string &input_path, const string &output_path) override;

    bool operator()(Container& a, Container& b) {
        if (portToIndexesInRoute.count(a.getDestinationPort())==0){
            portToIndexesInRoute[a.getDestinationPort()].push_back(INT32_MAX);
        }
        if (portToIndexesInRoute.count(b.getDestinationPort())==0){
            portToIndexesInRoute[b.getDestinationPort()].push_back(INT32_MAX);
        }
        return portToIndexesInRoute[a.getDestinationPort()][0] < portToIndexesInRoute[b.getDestinationPort()][0]; // private
    }

    void sortContainers(vector<Container>& containers);

    /* find the lowest position in port which
     * contains a container of specific destination*/
    int findLowestPlaceOfPortInPosition(const string &port, ContainersPosition &position); // private

    int getLoadInstructions(const string &input_path, const string &output_path); // private

    int getUnloadInstructions(const string &output_path); // private
};


#endif //CPP_STOWAGE_NAIVEALGORITHM_H
