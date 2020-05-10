//
//
// Created by Oz Zafar on 11/04/2020.

#include <fstream>
#include <sstream>
#include <map>
#include <utility>
#include "CraneManagement.h"
#include "../utils/IO.h"
#include "../../interfaces/AbstractAlgorithm.h"


CraneManagement::CraneManagement(string errorsFilePath) : errorsFilePath(std::move(errorsFilePath)) { }

int CraneManagement::load(Ship* ship, string& containerId, int floor, int row, int column) {
    ShipPlan& shipPlan = ship->getShipPlan();
    ContainersPosition &position = shipPlan.getContainerPosition(row, column);
    if (position.getTopFloorNumber()!=floor-1){
        fout.open(errorsFilePath,std::fstream::app);
        if (fout.is_open()) {
            fout << "Error: can't load to floor " << floor << " in position " << "("<<row << " " << column << "),";
        }
        fout.close();
        return 0;
    }
    return position.load(containerId, true);
}


int CraneManagement::unload(Ship* ship, string &containerId, int floor,int row, int column) {
    ShipPlan& shipPlan = ship->getShipPlan();
    ContainersPosition &position = shipPlan.getContainerPosition(row, column);
    if (position.getTopFloorNumber()!=floor){
        fout.open(errorsFilePath,std::fstream::app);
        if (fout.is_open()) {
            fout << "Error: can't unload the container " << containerId << " from floor " << floor << " in position "
                 << "(" << row << " " << column << ") because the container isn't on top,";
        }
        return 0;
    }
    return shipPlan.getContainerPosition(row, column).unload(containerId, true);
}
int CraneManagement::move(Ship* ship, string &containerId, int oldFloor, int oldRow, int oldColumn, int newRow, int newColumn, int newFloor) {
    int unload = CraneManagement::unload(ship,containerId,oldFloor,oldRow,oldColumn);
    if (unload){
        return CraneManagement::load(ship,containerId,newFloor,newRow,newColumn);
    }
    return 0;
}

/* ignores extra param in line
 * print error message is line has less params then expected */
CraneManagement::CraneManagementAnswer CraneManagement::readAndExecuteInstructions(Ship * ship, const string &input_path) {
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
                continue;
            }

            switch (command) {
                case (char) Action::LOAD :
                    if (row.size() < 5) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                    }else{
                        string containerId = row[1];
                        if (load(ship, containerId, stoi(row[2]), stoi(row[3]), stoi(row[4]))){
                            changedContainers[Action::LOAD].push_back(containerId);
                            count++;
                            string destination = ship->containerIdToDestination(containerId);
                            ship->portToContainers[destination].insert(containerId);
                        }
                    }
                    break;
                case (char) Action::UNLOAD :
                    if (row.size() < 5) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                    } else{
                        string containerId = row[1];
                        if (unload(ship, containerId, stoi(row[2]), stoi(row[3]), stoi(row[4]))){
                            changedContainers[Action::UNLOAD].push_back(containerId);
                            count++;
                            string destination = ship->containerIdToDestination(containerId);
                            ship->portToContainers[destination].erase(containerId);
                        }
                    }
                    break;
                case (char) Action::REJECT :
                    if (row.size() < 2) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                    } else {
                        changedContainers[Action::REJECT].push_back(row[1]);
                    }
                    break;
                case (char) Action::MOVE :
                    if (row.size() < 8) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                    } else {
                        if (move(ship, row[1], stoi(row[2]), stoi(row[3]), stoi(row[4]), stoi(row[4]), stoi(row[5]),
                             stoi(row[6]))){
                            count++;
                            changedContainers[Action::MOVE].push_back(row[1]);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
    return {changedContainers,count};
}

