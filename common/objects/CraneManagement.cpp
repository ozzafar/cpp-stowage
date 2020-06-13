//
//
// Created by Oz Zafar on 11/04/2020.

#include <sstream>
#include <map>
#include <utility>
#include "CraneManagement.h"
#include "../utils/IO.h"
#include "../../interfaces/AbstractAlgorithm.h"
#include "../utils/Errors.h"


#define SUCCESS (int)Error::SUCCESS

int CraneManagement::load(Ship& ship, string& containerId, int floor, int row, int column) {
    Errors errors;
    if(row < 0 || column < 0 || row >= ship.getShipPlan().getPlanLength() || column >= ship.getShipPlan().getPlanWidth()) {
        errors.addError(Error::ALGORITHM_NOT_ALLOWED_INSTRUCTION,
                        std::to_string(row) + "," + std::to_string(column) + " illegal position in ship");
        return errors.getErrorsCode();
    }
    ShipPlan& shipPlan = ship.getShipPlan();
    ContainersPosition &position = shipPlan.getContainerPosition(row, column);
    if (position.getTopFloorNumber()!=floor-1){
        errors.addError(Error::ALGORITHM_NOT_ALLOWED_INSTRUCTION,"there is no place to load containers in position "+
                        std::to_string(row) + "," + std::to_string(column));
        return errors.getErrorsCode();
    }

    position.load(containerId, true);
    return errors.getErrorsCode();
}


int CraneManagement::unload(Ship& ship, string &containerId, int floor,int row, int column) {
    Errors errors;
    ShipPlan& shipPlan = ship.getShipPlan();
    if(row < 0 || column < 0 || row >= ship.getShipPlan().getPlanLength() || column >= ship.getShipPlan().getPlanWidth())
    {
        errors.addError(Error::ALGORITHM_NOT_ALLOWED_INSTRUCTION,
                        std::to_string(row) + "," + std::to_string(column) + " illegal position in ship");
        return errors.getErrorsCode();
    }

    if(!ship.knowContainerId(containerId))
    {
        errors.addError(Error::ALGORITHM_NOT_ALLOWED_INSTRUCTION,"container id "+containerId+ " is unknown");
        return errors.getErrorsCode();
    }

    ContainersPosition &position = shipPlan.getContainerPosition(row, column);
    if (position.getTopFloorNumber()!=floor){
        errors.addError(Error::ALGORITHM_NOT_ALLOWED_INSTRUCTION,"in position "+ std::to_string(row) + "," + std::to_string(column) + " the top position top floor isn't " + std::to_string(floor) + " as expected");
        return errors.getErrorsCode();
    }
    shipPlan.getContainerPosition(row, column).unload(containerId, true);
    return errors.getErrorsCode();
}
int CraneManagement::move(Ship& ship, string &containerId, int oldFloor, int oldRow, int oldColumn,int newFloor, int newRow, int newColumn) {
    Errors errors;
    if(newRow < 0 || newColumn < 0 || newRow >= ship.getShipPlan().getPlanLength() || newColumn >= ship.getShipPlan().getPlanWidth()
    || oldRow < 0 || oldColumn < 0 || oldRow >= ship.getShipPlan().getPlanLength() || oldColumn >= ship.getShipPlan().getPlanWidth())
    {
        errors.addError(Error::ALGORITHM_NOT_ALLOWED_INSTRUCTION,"illegal position in ship");
        return errors.getErrorsCode();
    }

//    if(!ship.knowContainerId(containerId))
//    {
//        errors.addError(Error::ALGORITHM_UNKNOWN_CONTAINER_ID);
//        return errors.getErrorsCode();
//    }

    errors.addErrors(CraneManagement::unload(ship,containerId,oldFloor,oldRow,oldColumn));
    if (!errors.hasError()){
        errors.addErrors(CraneManagement::load(ship,containerId,newFloor,newRow,newColumn));
    }
    return errors.getErrorsCode();
}

/* ignores extra param in line
 * print error message is line has less params then expected */
CraneManagement::CraneManagementAnswer CraneManagement::readAndExecuteInstructions(Ship& ship, const string &input_path)
{
    Errors errors;
    string line;
    vector<string> row;
    std::ifstream instructionsFile(input_path);
    char command;
    map<Action ,vector<string>> changedContainers;
    int count = 0;

    if (instructionsFile.is_open()) {
        while (getline(instructionsFile, line)) {
            row = IO::breakLineToWords(line, ',');
            if (row[0].size() == 1) {
                command = row[0][0];
            } else{
                std::cout << "Error: invalid command to crane management" << std::endl;
                errors.addError(Error::ALGORITHM_INVALID_COMMAND);
                continue;
            }

            switch (command) {
                case (char) Action::LOAD :
                    if (row.size() < 5) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                        errors.addError(Error::ALGORITHM_INVALID_COMMAND);
                    }else{
                        string containerId = row[1];
                        errors.addErrors(load(ship, containerId, stoi(row[2]), stoi(row[3]), stoi(row[4])));
                        if (errors.getErrorsCode() == SUCCESS){
                            changedContainers[Action::LOAD].push_back(containerId);
                            count+=5;
                            string destination = ship.containerIdToDestination(containerId);
                            ship.portToContainers[destination].insert(containerId);
                        }
                    }
                    break;
                case (char) Action::UNLOAD :
                    if (row.size() < 5) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                        errors.addError(Error::ALGORITHM_INVALID_COMMAND);
                    } else{
                        string containerId = row[1];
                        errors.addErrors(unload(ship, containerId, stoi(row[2]), stoi(row[3]), stoi(row[4])));
                        if (errors.getErrorsCode() == SUCCESS){
                            changedContainers[Action::UNLOAD].push_back(containerId);
                            count+=5;
                            string destination = ship.containerIdToDestination(containerId);
                            ship.portToContainers[destination].erase(containerId);
                            ship.containerIdToContainer.erase(containerId);
                        }
                    }
                    break;
                case (char) Action::REJECT :
                    if (row.size() < 2) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                        errors.addError(Error::ALGORITHM_INVALID_COMMAND);
                    } else {
                        changedContainers[Action::REJECT].push_back(row[1]);
                    }
                    break;
                case (char) Action::MOVE :
                    if (row.size() < 8) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                        errors.addError(Error::ALGORITHM_INVALID_COMMAND);
                    } else {
                        errors.addErrors(move(ship, row[1], stoi(row[2]), stoi(row[3]), stoi(row[4]), stoi(row[5]), stoi(row[6]),
                                              stoi(row[7])));
                        if (errors.getErrorsCode() == SUCCESS){
                            count+=3;
                            changedContainers[Action::MOVE].push_back(row[1]);
                        }
                    }
                    break;
                default:{
                    std::cout << "Error: invalid command letter: " << command << std::endl;
                    errors.addError(Error::ALGORITHM_INVALID_COMMAND);
                    break;
                }
            }
        }
    }
    return {changedContainers,count, errors.getErrorsCode()};
}

