//
// Created by Oz Zafar on 03/05/2020.
//

#include "BasicAlgorithm.h"
#include <algorithm>
#include "../common/objects/ContainersPosition.h"
#include "../common/utils/IO.h"
#include "../common/utils/Errors.h"
#include "NaiveAlgorithm.h"
#include "../simulation/CheckAlgorithmError.h"
#include <fstream>
#include <iostream>

using std::stringstream;
using std::string;
using std::vector;

// region CLASS FUNCTIONS

void BasicAlgorithm::writeOperation(const std::string& filename, AbstractAlgorithm::Action op, const string& containerId, int floor, int x, int y)
{
    std::ofstream fout;
    fout.open(filename,std::fstream::app);
    if (fout.is_open())
    {
        if (op == AbstractAlgorithm::Action::LOAD || op == AbstractAlgorithm::Action::UNLOAD){
            fout << (char)op << "," << containerId << "," << floor << "," << x << "," << y << '\n';
        }
        else if (op == AbstractAlgorithm::Action::REJECT){
            fout << (char)op << "," << containerId << '\n';
        }
        else{
            // op == AbstractAlgorithm::Action::MOVE
            // TODO - complete for HW3
        }
        fout.close();
    }
    else {
        std::cout << "Unable to open file";
    }
}

int BasicAlgorithm::readContainerAwaitingAtPortFile(const string &path,vector<Container>& waitingContainers, vector<Container>& badContainers) {
    Errors errors;
    errors.addError(IO::readContainerAwaitingAtPortFile(path, ship, waitingContainers, badContainers));
    if (this->route.inLastStop() && (!waitingContainers.empty() || !badContainers.empty()))
    {
        // CheckAlgorithmError::SHIP_HAS_CONTAINERS_AT_THE_END_OF_THE_ROUTE); TODO handle this
    }
    return errors.getErrorsCode();
}

Ship BasicAlgorithm::getShip() {
    return ship;
}

Route BasicAlgorithm::getShipRoute() {
    return route;
}

// endregion

// region OVERRIDE FUNCTIONS

int BasicAlgorithm::readShipPlan(const std::string &full_path_and_file_name)  {
    Errors errors;
    errors.addErrors(IO::readShipPlan(full_path_and_file_name,ship.getShipPlan()));
    return errors.getErrorsCode();
}

int BasicAlgorithm::readShipRoute(const string &path) {
    Errors errors;
    errors.addErrors(IO::readShipRoute(path,route));
    for (size_t i = 0 ; i < route.getPorts().size() ; i++){
        portToIndexesInRoute[route.getPorts().at(i).substr(0,5)].push_back(i); // TODO remove substr !
    }
    return errors.getErrorsCode();
}

int BasicAlgorithm::setWeightBalanceCalculator(WeightBalanceCalculator &weightBalanceCalculator) {
    calculator = weightBalanceCalculator;
    return (int)(Error::SUCCESS);
}

// endregion
