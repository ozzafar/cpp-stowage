//
// Created by Oz Zafar on 11/04/2020.
//

#ifndef CPP_STOWAGE_CRANEMANAGEMENT_H
#define CPP_STOWAGE_CRANEMANAGEMENT_H

#include <map>
#include "ShipPlan.h"
#include "Ship.h"
#include "CraneOperation.h"
#include "CraneOperation.h"
#include <fstream>

using std::map;

class CraneManagement {

    std::ofstream fout;

    string errorsFilePath;

public:

    CraneManagement(string errorsFilePath) ;

    // return value is SUCCESS / FAILURE:
    int load(Ship* ship, string &containerId, int floor, int row, int column);

    int unload(Ship* ship, string &containerId, int floor, int row, int column);

    int move(Ship* ship, string &containerId, int oldFloor, int oldRow, int oldColumn, int newRow, int newColumn,
         int newFloor);


    typedef struct {
        map<CraneOperation,vector<string>> changedContainers;
        int numOfOperations;
    } CraneManagementAnswer;

    CraneManagementAnswer readAndExecuteInstructions(Ship* ship, const string &input_path);
};

#endif //CPP_STOWAGE_CRANEMANAGEMENT_H
