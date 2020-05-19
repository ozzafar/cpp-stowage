//
// Created by Oz Zafar on 11/04/2020.
//

#ifndef CPP_STOWAGE_CRANEMANAGEMENT_H
#define CPP_STOWAGE_CRANEMANAGEMENT_H

#include <map>
#include "ShipPlan.h"
#include "Ship.h"
#include "../../interfaces/AbstractAlgorithm.h"
#include "../utils/Errors.h"
#include <fstream>

using std::map;
typedef AbstractAlgorithm::Action Action;

class CraneManagement {

public:

    // return value is SUCCESS / FAILURE:
    int load(Ship& ship, string &containerId, int floor, int row, int column);

    int unload(Ship& ship, string &containerId, int floor, int row, int column);

    int move(Ship& ship, string &containerId, int oldFloor, int oldRow, int oldColumn, int newRow, int newColumn,
         int newFloor);


    typedef struct {
        map<Action,vector<string>> changedContainers;
        int numOfOperations;
        int errors;
    } CraneManagementAnswer;

    CraneManagementAnswer readAndExecuteInstructions(Ship& ship, const string &input_path);
};

#endif //CPP_STOWAGE_CRANEMANAGEMENT_H
