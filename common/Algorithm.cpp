//
// Created by Oz Zafar on 09/04/2020.
//

#include "Algorithm.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "ContainersPosition.h"
#include "CraneOperation.h"

using std::stringstream;
using std::string;

void Algorithm::readShipPlan(const string &path) {
    int numOfFloors, X, Y;
    string line,word;
    std::ifstream planFile(path);
    vector<string> row;

    getline(planFile, line);
    breakLineToWords(line, row);
    numOfFloors = stoi(row[0]);
    X = stoi(row[1]);
    Y = stoi(row[2]);
    vector<vector<ContainersPosition>> plan(X, vector<ContainersPosition>(Y,ContainersPosition()));
    shipPlan = new ShipPlan(numOfFloors,plan);

    int x, y, actualNumOfFloors;
    if (planFile.is_open()) {
        while (getline(planFile, line)) {
            breakLineToWords(line, row);
            x = stoi(row[0]);
            y = stoi(row[1]);
            actualNumOfFloors = stoi(row[2]);
            if (actualNumOfFloors==numOfFloors){
                std::cout << "Warning: in position (" << x << "," << y << ") the actual number of floors: "<< actualNumOfFloors
                << " is illegal (max number is " << numOfFloors -1 << ")" << std::endl;
            }
            else if (x < 0 || x >= X || y < 0 || y>=Y){
                std::cout << "Warning: the position (" << x << "," << y << ") is illegal"<< std::endl;
            } else {
                shipPlan->setStartFloorInPosition(x, y, numOfFloors - actualNumOfFloors);
            }
        }
        planFile.close();
    }
}

void Algorithm::breakLineToWords(string &line, vector<string> &row) {
    string word;
    row.clear();
    stringstream ss(line);
    while (getline(ss, word, ',')){
        row.push_back(word);
    }
}

void Algorithm::readShipRoute(const string &path) {
}

ShipPlan* Algorithm::getShipPlan() {
    return shipPlan;
}


// input_path is Containers-awaiting-at-port file
void Algorithm::getInstructionsForCargo(const string &input_path, const string &output_path) {
    vector<Container*> containers = readContainerAwaitingAtPortFile(input_path);
    updateContainerMapping(containers);
    int amount = containers.size(), index = 0;
    for (int i = 0; i < shipPlan->getPlanLength(); ++i) {
        for (int j = 0; j < shipPlan->getPlanWidth(); ++j) {
            ContainersPosition containersPosition = shipPlan->getContainerPosition(i, j);
            for (int k = 0 ; k < containersPosition.getNumOfActiveFloors(); k++) {
                if (index < amount && calculator.tryOperation((char)CraneOperation::LOAD,containers.at(index)->getWeight(),i,j)){

                }
                index++;
            }
        }
    }
}

vector<Container*> Algorithm::readContainerAwaitingAtPortFile(const string &path) {
    string line;
    std::ifstream planFile(path);
    vector<string> row;
    vector<Container*> containers;

    if (planFile.is_open()) {
        while (getline(planFile, line)) {
            breakLineToWords(line, row);
            string containerId = row[0];
            int weight = stoi(row[1]);
            string destinationPort = row[2];
            containers.push_back(new Container(weight,destinationPort,containerId));
        }
        planFile.close();
    }
    return containers;
}

void Algorithm::updateContainerMapping(vector<Container*> containers) {
    for (auto container : containers){
        this->containerIdToContainer[container->getId()] = container;
    }
}

