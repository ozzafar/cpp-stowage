//
// Created by Oz Zafar on 03/05/2020.
//

#ifndef CPP_STOWAGE_BASICALGORITHM_H
#define CPP_STOWAGE_BASICALGORITHM_H

#include "../interfaces/AbstractAlgorithm.h"
#include "../common/objects/Ship.h"
#include "../common/objects/Route.h"
#include "WeightBalanceCalculator.h"
#include <string>

using std::string;

class BasicAlgorithm : public AbstractAlgorithm {

protected:

    WeightBalanceCalculator calculator;
    Ship ship;
    Route route;
    map<string, vector<int>> portToIndexesInRoute;

public:

    // class functions
    int readContainerAwaitingAtPortFile(const string &path,vector<Container>& waitingContainers);
    void writeOperation(const std::string &filename, AbstractAlgorithm::Action op, const string &containerId, int floor, int x, int y);  // writes single operation to output file
    Ship getShip();
    Route getShipRoute();
    static AbstractAlgorithm *createAlgorithmByName(string basicString);

    // override functions
    int readShipPlan(const std::string& full_path_and_file_name) override;
    int readShipRoute(const std::string& full_path_and_file_name) override;
    int setWeightBalanceCalculator(WeightBalanceCalculator& calculator) override;
    int getInstructionsForCargo(const std::string& input_full_path_and_file_name,const std::string& output_full_path_and_file_name) override = 0 ;


};

#endif //CPP_STOWAGE_BASICALGORITHM_H
