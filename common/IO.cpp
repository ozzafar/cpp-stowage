//
// Created by Oz Zafar on 18/04/2020.
//

#include <fstream>
#include <sstream>
#include "IO.h"

//region HELP FUNCTIONS

static bool isAlphabetString(const string &str) {
    char ch;
    for (char i : str) {
        ch = i;
        if ((ch < 65) || (ch > 122) || (ch > 90 && ch < 97)) {
            return false;
        }
    }
    return true;
}

static void createPositionFromRowInput(int numOfFloors, int X, int Y, string &line, Ship* ship) {
    int x, y, actualNumOfFloors;
    vector<string> row = IO::breakLineToWords(line, ',');
    if (row.size() != 3){
        std::cout << "Warning: bad input" << std::endl;
    } else {
        x = stoi(row[0]);
        y = stoi(row[1]);
        actualNumOfFloors = stoi(row[2]);
        if (actualNumOfFloors > numOfFloors) {
            std::cout << "Warning: in position (" << x << "," << y << ") the actual number of floors: "
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
}

static bool checkPortNumberInput(vector<string> portNumber) {
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

// endregion

// region MAIN FUNCTIONS

vector<Container*> IO::readContainerAwaitingAtPortFile(const string &input_path, Ship* ship) {
    string line;
    std::ifstream planFile(input_path);
    vector<string> row;
    vector<Container*> containers;

    if (planFile.is_open()) {
        while (getline(planFile, line)) {
            row = breakLineToWords(line,',');
            if(row.empty())
            {
                continue;
            }
            string containerId = row[0];
            if (!Container::isValidID(containerId) && 0){ //TODO remove && 0
                std::cout << "Bad input: invalid container id" << std::endl;
                continue;
            }
            int weight = stoi(row[1]);
            string destinationPort = row[2];

            if (!isAlphabetString(destinationPort)){
                std::cout << "Bad input: line in ContainerAwaitingAtPort input file ignored because container id contains non alphabet chars" << std::endl;
                continue;
            }
            if (destinationPort.size() != 5) {
                std::cout << "Bad input: line in ContainerAwaitingAtPort input file ignored because container id isn't in the correct size" << std::endl;
                continue;
            }

            if (ship->knowContainerId(containerId)){
                std::cout << "Warning: container " << containerId << " appears twice" << std::endl;
            }
            else{
                auto* newContainer = new Container(weight,destinationPort,containerId);
                containers.push_back(newContainer);
                ship->updateContainerMapping(new Container(weight,destinationPort,containerId));
            }
        }
        planFile.close();
    }
    return containers;
}

void IO::readShipPlan(const string &path, Ship* ship) {
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
        ship->setShipPlan(ShipPlan(plan));

        while (getline(planFile, line)) {
            createPositionFromRowInput(numOfFloors, X, Y, line, ship);
        }
        planFile.close();
    }
}

Route* IO::readShipRoute(const string &path) {
    vector<string> ports;
    int rowIndex = 0;
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
            std::transform(row[0].begin(), row[0].end(), row[0].begin(), ::toupper);
            ports.push_back(row[0]);
        }
        planFile.close();
    }
    return new Route(ports);
}

vector<string> IO::breakLineToWords(string &line, char delimeter) {
    string word;
    vector<string>  row;
    std::stringstream ss(line);
    ws(ss);
    while (getline(ss, word, delimeter)){
        row.push_back(word);
    }
    return row;
}

// endregion