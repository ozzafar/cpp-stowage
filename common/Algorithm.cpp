//
// Created by Oz Zafar on 09/04/2020.
//

#include "Algorithm.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "ContainersPosition.h"

using std::stringstream;

Algorithm::Algorithm() {}

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
                << "is illegal (max number is " << numOfFloors -1 << std::endl;
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

