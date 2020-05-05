//
// Created by Oz Zafar on 13/04/2020.
//

#ifndef CPP_STOWAGE_ABSTRACTALGORITHM_H
#define CPP_STOWAGE_ABSTRACTALGORITHM_H

#include "../objects/ShipPlan.h"
#include "../../src/algorithm/WeightBalanceCalculator.h"
#include "../objects/Container.h"
#include "../objects/Route.h"
#include "../objects/Ship.h"
#include <string>
#include <map>

class WeightBalanceCalculator;

class AbstractAlgorithm {

public:

    virtual ~AbstractAlgorithm(){}

    enum class Action { LOAD = 'L', UNLOAD = 'U', MOVE = 'M', REJECT = 'R'};

    virtual int readShipPlan(const std::string& full_path_and_file_name) = 0;
    virtual int readShipRoute(const std::string& full_path_and_file_name) = 0;
    virtual int setWeightBalanceCalculator(WeightBalanceCalculator& calculator) = 0;
    virtual int getInstructionsForCargo(
            const std::string& input_full_path_and_file_name,
            const std::string& output_full_path_and_file_name) = 0;
};


#endif //CPP_STOWAGE_ABSTRACTALGORITHM_H
