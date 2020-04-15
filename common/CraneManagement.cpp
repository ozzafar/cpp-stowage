//
// Created by Oz Zafar on 11/04/2020.
//

#include <fstream>
#include <sstream>
#include "CraneManagement.h"
#include "CraneOperation.h"

vector<string> breakLineToWords2(string &line, char delimeter) {
    string word;
    vector<string> row;
    std::stringstream ss(line);
    ws(ss);
    while (getline(ss, word, delimeter)){
        row.push_back(word);
    }
    return row;
}

int CraneManagement::load(ShipPlan& shipPlan, string& containerId, int floor, int row, int column) {
    ContainersPosition &position = shipPlan.getContainerPosition(row, column);
    if (position.getTopFloorNumber()!=floor-1){
        std::cout << "Error: can't load to floor " << floor << " in position " << "("<<row << "," << column << ")" << std::endl;
        return FAILURE;
    }
    return position.load(containerId);
}

int CraneManagement::unload(ShipPlan& shipPlan, string &containerId, int floor,int row, int column) {
    ContainersPosition &position = shipPlan.getContainerPosition(row, column);
    if (position.getTopFloorNumber()!=floor-1){
        std::cout << "Error: can't unload the container " << containerId << " from floor " << floor << " in position " << "("<<row << "," << column << "), because the container isn't on top" << std::endl;
        return FAILURE;
    }
    return shipPlan.getContainerPosition(row, column).unload(containerId);
}

int CraneManagement::move(ShipPlan &shipPlan, string &containerId, int oldFloor, int oldRow, int oldColumn, int newRow, int newColumn, int newFloor) {
    int unload = shipPlan.getContainerPosition(oldRow, oldColumn).unload(containerId);
    if (unload){
        return shipPlan.getContainerPosition(newRow, newColumn).load(containerId);
    }
    return FAILURE;
}

void CraneManagement::readAndExecuteInstructions(ShipPlan &shipPlan, const string &input_path) {
    string line;
    vector<string> row;
    std::ifstream instructionsFile(input_path);
    char command;

    if (instructionsFile.is_open()) {
        while (getline(instructionsFile, line)) {
            row = breakLineToWords2(line, ',');
            if (row[0].size() == 1) {
                command = row[0][0];
            } else{
                std::cout << "Error: invalid command to crane management" << std::endl;
                continue;
            }

            switch (command) {
                case (char) CraneOperation::LOAD :
                    if (row.size() != 5) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                        return;
                    }
                    load(shipPlan, row[1], stoi(row[2]), stoi(row[3]), stoi(row[4]));
                    break;
                case (char) CraneOperation::UNLOAD :
                    if (row.size() != 5) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                        return;
                    }
                    unload(shipPlan, row[1], stoi(row[2]), stoi(row[3]), stoi(row[4]));
                    break;
                case (char) CraneOperation::REJECT :
                    if (row.size() >= 2) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                        return;
                    }
                    break;
                case (char) CraneOperation::MOVE :
                    if (row.size() != 8) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                        return;
                    }
                    move(shipPlan, row[1], stoi(row[2]), stoi(row[3]), stoi(row[4]), stoi(row[4]), stoi(row[5]),
                         stoi(row[6]));
                    break;
                default:
                    break;
            }
        }
    }
}





