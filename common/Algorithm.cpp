//
// Created by Oz Zafar on 13/04/2020.
//

#include "Algorithm.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "ContainersPosition.h"
#include "CraneOperation.h"
#include <string>
#include <algorithm>

using std::stringstream;
using std::string;
using std::vector;

Algorithm::~Algorithm() {}

// region HELP FUNCTIONS

vector<string> breakLineToWords(string &line, char delimeter) {
    string word;
    vector<string>  row;
    stringstream ss(line);
    ws(ss);
    while (getline(ss, word, delimeter)){
        row.push_back(word);
    }
    return row;
}

void print(vector<string> const &list)
{
    for (auto const& i: list) {
        std::cout << i << "\n";
    }
}

bool isAlphabetString(const string &str) {
    char ch;
    for (char i : str) {
        ch = i;
        if ((ch < 65) || (ch > 122) || (ch > 90 && ch < 97)) {
            return false;
        }
    }
    return true;
}

bool checkPortNumberInput(vector<string> portNumber) {
    if (portNumber.empty() == 1) {
        std::cout << "line in route input file ignored because it is an empty line" << std::endl;
        return false;
    }

    if (portNumber[0].at(0) == '#') {
        std::cout << "line in route input file ignored because it is a comment" << std::endl;
        return false;
    }

    if (portNumber.size() != 1) {
        std::cout << "Bad input: line in route input file ignored because it didn't contain one word" << std::endl;
        return false;
    }

    if (portNumber[0].size() != 5) {
        std::cout << "Bad input: line in route input file ignored because it has no 5 chars" << std::endl;
        return false;
    }

    if (!isAlphabetString(portNumber[0])) {
        std::cout << "Bad input: line in route input file ignored because it contains non alphabet chars" << std::endl;
        return false;
    }

    return true;
}

string & Algorithm::createPositionFromRowInput(int numOfFloors, int X, int Y, string &line) const {
    int x, y, actualNumOfFloors;
    vector<string> row = breakLineToWords(line, ',');
    if (row.size() != 3){
        std::cout << "Warning: bad input" << std::endl;
    } else {
        x = stoi(row[0]);
        y = stoi(row[1]);
        actualNumOfFloors = stoi(row[2]);
        if (actualNumOfFloors > numOfFloors) {
            std::cout << "Error: in position (" << x << "," << y << ") the actual number of floors: "
                      << actualNumOfFloors << " is illegal (max number is " << numOfFloors << ")"
                      << std::endl;
        }
        else if (actualNumOfFloors == numOfFloors) {
            std::cout << "Warning: in position (" << x << "," << y << ") the actual number of floors: "
                      << actualNumOfFloors << " is the maximum number (" << numOfFloors << ")"
                      << std::endl;
        } else if (x < 0 || x >= X || y < 0 || y >= Y) {
            std::cout << "Warning: the position (" << x << "," << y << ") is illegal" << std::endl;
        } else {
            ship->getShipPlan().setStartFloorInPosition(x, y, numOfFloors - actualNumOfFloors);
        }
    }
    return line;
}

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
    int numOfFloors, X, Y;
    string line, word;
    std::ifstream planFile(path);
    vector<string> row;

    if (planFile.is_open()) {
        getline(planFile, line);
        row = breakLineToWords(line, ',');
        numOfFloors = stoi(row[0]);
        X = stoi(row[1]);
        Y = stoi(row[2]);
        vector<vector<ContainersPosition>> plan(X, vector<ContainersPosition>(Y, ContainersPosition(numOfFloors)));
        ship->setShipPlan(*new ShipPlan(numOfFloors, plan));

        while (getline(planFile, line)) {
            line = createPositionFromRowInput(numOfFloors, X, Y, line);
        }
        planFile.close();
    }
}

void Algorithm::readShipRoute(const string &path) {
    vector<string> ports;
    int rowIndex=  0;
    string line;
    vector<string> row;

    std::ifstream planFile(path);

    if (planFile.is_open()) {
        while (getline(planFile, line)) {
            rowIndex++;
            std::cout << "Reading input route file, line " << rowIndex << std::endl;
            row = breakLineToWords(line, ' ');

            if (!checkPortNumberInput(row)) {
                continue;
            }
            row[0]= row[0].substr(0,5); //TODO fix this
            std::transform(row[0].begin(), row[0].end(), row[0].begin(), ::toupper);
            ports.push_back(row[0]);
            portToIndexesInRoute[row[0]].push_back(rowIndex);
        }
        planFile.close();
    }
    shipRoute = new Route(ports);
}

vector<Container*> Algorithm::readContainerAwaitingAtPortFile(const string &path) {
    string line;
    std::ifstream planFile(path);
    vector<string> row;
    vector<Container*> containers;

    if (planFile.is_open()) {
        while (getline(planFile, line)) {
            row = breakLineToWords(line,',');
            string containerId = row[0];
            int weight = stoi(row[1]);
            string destinationPort = row[2];
            //TODO fix this
//            if (!isAlphabetString(destinationPort)){
//                std::cout << "Bad input: line in ContainerAwaitingAtPort input file ignored because container id contains non alphabet chars" << std::endl;
//                continue;
//            }
//            if (!destinationPort.size() == 5) {
//                std::cout << "Bad input: line in ContainerAwaitingAtPort input file ignored because container id isn't in the correct size" << std::endl;
//                continue;
//            }
            //TODO fix this

            destinationPort = destinationPort.substr(0,5); // remove \n TODO fix this
            if (ship->knowContainerId(containerId)){
                std::cout << "Warning: container " << containerId << " appears twice" << std::endl;
            }
            else{
                Container* newContainer = new Container(weight,destinationPort,containerId);
                containers.push_back(newContainer);
                ship->updateContainerMapping(new Container(weight,destinationPort,containerId));
            }
        }
        planFile.close();
    }
    return containers;
}

Ship *Algorithm::getShip() const {
    return ship;
}

Route *Algorithm::getShipRoute() const {
    return shipRoute;
}

void Algorithm::setCalculator(WeightBalanceCalculator &calculator) {
    Algorithm::calculator = calculator;
}


// endregion
