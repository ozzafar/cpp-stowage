//

#include <fstream>
#include <sstream>
#include <map>
#include <utility>
#include "CraneManagement.h"
#include "CraneOperation.h"

//
// Created by Oz Zafar on 11/04/2020.


CraneManagement::CraneManagement(string filename) : errorsFilePath(std::move(errorsFilePath)) {}

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
        fout.open(errorsFilePath,std::fstream::app);
        if (fout.is_open()) {
            fout << "Error: can't load to floor " << floor << " in position " << "("<<row << " " << column << "),";
        }
        fout.close();
        return FAILURE;
    }
    return position.load(containerId);
}


int CraneManagement::unload(ShipPlan& shipPlan, string &containerId, int floor,int row, int column) {
    ContainersPosition &position = shipPlan.getContainerPosition(row, column);
    if (position.getTopFloorNumber()!=floor-1){
        fout.open(errorsFilePath,std::fstream::app);
        if (fout.is_open()) {
            fout << "Error: can't unload the container " << containerId << " from floor " << floor << " in position "
                 << "(" << row << " " << column << ") because the container isn't on top,";
        }
        return FAILURE;
    }
    return shipPlan.getContainerPosition(row, column).unload(containerId);
}

int CraneManagement::move(ShipPlan &shipPlan, string &containerId, int oldFloor, int oldRow, int oldColumn, int newRow, int newColumn, int newFloor) {
    int unload = CraneManagement::unload(shipPlan,containerId,oldFloor,oldFloor,oldColumn);
    if (unload){
        return CraneManagement::load(shipPlan,containerId,newFloor,newRow,newColumn);
    }
    return FAILURE;
}

/* ignores extra param in line
 * print error message is line has less params then expected */
CraneManagement::CraneManagementAnswer CraneManagement::readAndExecuteInstructions(ShipPlan &shipPlan, const string &input_path) {
    string line;
    vector<string> row;
    std::ifstream instructionsFile(input_path);
    char command;
    map<string,CraneOperation> changedContainers;
    int count = 0;

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
                    if (row.size() < 5) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                    }else{
                        if (load(shipPlan, row[1], stoi(row[2]), stoi(row[3]), stoi(row[4]))){
                            if (changedContainers.count(row[1])>0){
                                changedContainers.erase(row[1]);
                            }
                            else{
                                changedContainers[row[1]] = CraneOperation::LOAD;
                            }
                            count++;
                        }
                    }
                    break;
                case (char) CraneOperation::UNLOAD :
                    if (row.size() < 5) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                    } else{
                        if (unload(shipPlan, row[1], stoi(row[2]), stoi(row[3]), stoi(row[4]))){
                            changedContainers[row[1]] = CraneOperation::UNLOAD;
                            count++;
                        }
                    }
                    break;
                case (char) CraneOperation::REJECT :
                    if (row.size() < 2) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                    }
                    break;
                case (char) CraneOperation::MOVE :
                    if (row.size() < 8) {
                        std::cout << "Error: invalid number of arguments for command: " << command << std::endl;
                    } else {
                        if (move(shipPlan, row[1], stoi(row[2]), stoi(row[3]), stoi(row[4]), stoi(row[4]), stoi(row[5]),
                             stoi(row[6]))){
                            count++;
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
