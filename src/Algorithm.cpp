//
// Created by Oz Zafar on 13/04/2020.
//

#include "Algorithm.h"
#include <fstream>
#include <iostream>
#include "ContainersPosition.h"
#include "CraneOperation.h"
#include "../common/IO.h"
#include <algorithm>

using std::stringstream;
using std::string;
using std::vector;

Algorithm::~Algorithm() = default;

// region HELP FUNCTIONS

void Algorithm::writeOperation(const std::string& filename, CraneOperation op, const string& containerId, int floor, int x, int y)
{
    std::ofstream fout;
    fout.open(filename,std::fstream::app);
    if (fout.is_open())
    {
        if (op == CraneOperation::LOAD || op == CraneOperation::UNLOAD){
            fout << (char)op << "," << containerId << "," << floor << "," << x << "," << y << '\n';
        }
        else if (op == CraneOperation::REJECT){
            fout << (char)op << "," << containerId << '\n';
        }
        else{
            // op == CraneOperation::MOVE
            // TODO - complete for HW2
        }
        fout.close();
    }
    else {
        std::cout << "Unable to open file";
    }
}

// endregion

// region MAIN FUNCTIONS

void Algorithm::readShipPlan(const string &path) {
    IO::readShipPlan(path,ship);
}

void Algorithm::readShipRoute(const string &path) {
    shipRoute = IO::readShipRoute(path);
    for (size_t i = 0 ; i < shipRoute->getPorts().size() ; i++){
        portToIndexesInRoute[shipRoute->getPorts().at(i)].push_back(i);
    }
}

Ship *Algorithm::getShip() const {
    return ship;
}

Route *Algorithm::getShipRoute() const {
    return shipRoute;
}

void Algorithm::setCalculator(WeightBalanceCalculator &calculatorP) {
    Algorithm::calculator = calculatorP;
}

vector<Container *> Algorithm::readContainerAwaitingAtPortFile(const string &path) {
    return IO::readContainerAwaitingAtPortFile(path,ship);
}

// endregion
