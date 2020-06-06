//
// Created by Oz Zafar on 03/05/2020.
//

#include "BasicAlgorithm.h"


using std::stringstream;
using std::string;
using std::vector;

// region CLASS FUNCTIONS

void BasicAlgorithm::writeOperation(const std::string& filename, AbstractAlgorithm::Action op, const string& containerId, int floor, int x, int y)
{
    // TODO dont open file every call to this function
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
        std::cout << "Unable to open file" << std::endl;
    }
}

int BasicAlgorithm::readContainerAwaitingAtPortFile(const string &path,vector<Container>& waitingContainers, vector<Container>& badContainers) {
    Errors errors;
    IO::readContainerAwaitingAtPortFile(path, ship, waitingContainers, badContainers,errors);
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
    IO::readShipPlan(full_path_and_file_name,ship.getShipPlan(),errors);
    return errors.getErrorsCode();
}

int BasicAlgorithm::readShipRoute(const string &path) {
    Errors errors;
    IO::readShipRoute(path,route,errors);
    for (size_t i = 0 ; i < route.getPorts().size() ; i++){
        portToIndexesInRoute[route.getPorts().at(i).substr(0,5)].push_back(i);
    }
    return errors.getErrorsCode();
}

int BasicAlgorithm::setWeightBalanceCalculator(WeightBalanceCalculator &weightBalanceCalculator) {
    calculator = weightBalanceCalculator;
    return (int)(Error::SUCCESS);
}

// endregion
