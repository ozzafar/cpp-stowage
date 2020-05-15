//
// Created by Oz Zafar on 18/04/2020.
//

#include <fstream>
#include <sstream>
#include "IO.h"
#include "Errors.h"
#include <algorithm>

//region HELP FUNCTIONS

static bool isAlphabetString(const string &str) {
    string strForCheck;

    if(iscntrl(str[str.size()-1]))
    {
        strForCheck = str.substr(0, str.size()-1);
    } else{
        strForCheck = str;
    }

    char ch;
    for (char i : strForCheck) {
        ch = i;
        if ((ch < 65) || (ch > 122) || (ch > 90 && ch < 97)) {
            return false;
        }
    }
    return true;
}

static int createPositionFromRowInput(int numOfFloors, int X, int Y, string &line, ShipPlan& shipPlan) {
    Errors errors;
    int x, y, actualNumOfFloors;
    vector<string> row = IO::breakLineToWords(line, ',');
    if (row.size() != 3){
        errors.addError(Error::PLAN_BAD_LINE_FORMAT_WARNING);
        std::cout << "Warning: bad input" << std::endl;
    } else {
        x = stoi(row[0]);
        y = stoi(row[1]);
        actualNumOfFloors = stoi(row[2]);
        if (actualNumOfFloors > numOfFloors) {
            errors.addError(Error::NUMBER_OF_FLOORS_WARNING);
            std::cout << "Warning: in position (" << x << "," << y << ") the actual number of floors: "
                      << actualNumOfFloors << " is illegal (max number is " << numOfFloors << ")"
                      << std::endl;
        }
        else if (actualNumOfFloors == numOfFloors) {
            errors.addError(Error::NUMBER_OF_FLOORS_WARNING);
            std::cout << "Warning: in position (" << x << "," << y << ") the actual number of floors: "
                      << actualNumOfFloors << " is the maximum number (" << numOfFloors << ")"
                      << std::endl;
        } else if (x < 0 || x >= X || y < 0 || y >= Y) {
            std::cout << "Warning: the position (" << x << "," << y << ") is illegal" << std::endl;
        } else {
            shipPlan.setStartFloorInPosition(x, y, numOfFloors - actualNumOfFloors);
        }
    }
    return errors.getErrorsCode();
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

    if(iscntrl(portNumber[0].at(portNumber[0].size()-1)))
    {
        portNumber[0] = portNumber[0].substr(0, portNumber[0].size()-1);
    }

    if (portNumber[0].size() != 5) {
        std::cout << "Bad input: line in route input file ignored because it has no 5 chars " << portNumber[0] << std::endl;
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

int IO::readContainerAwaitingAtPortFile(const string &input_path, Ship& ship, vector<Container>& waitingContainers) {
    Errors errors;
    string line;
    std::ifstream planFile(input_path);
    vector<string> row;

    if (planFile.is_open()) {
        while (getline(planFile, line)) {
            row = breakLineToWords(line,',');
            if(row.empty())
            {
                continue;
            }
            string containerId = row[0];


            if (!Container::isValidID(containerId)){
                errors.addError(Error::ILLEGAL_ID_CHECK_WARNING);
                std::cout << "Bad input: invalid container id " << containerId << std::endl;
                continue;
            }
            int weight = stoi(row[1]);
            string destinationPort = row[2];

            if(iscntrl(destinationPort.at(destinationPort.size()-1)))
            {
                destinationPort = destinationPort.substr(0, destinationPort.size()-1);
            }

            if (!isAlphabetString(destinationPort)){
                errors.addError(Error::ID_CANNOT_BE_READ_WARINING);
                std::cout << "Bad input: line in ContainerAwaitingAtPort input file ignored because container id contains non alphabet chars" << std::endl;
                continue;
            }
            if (destinationPort.size() != 5) {
                errors.addError(Error::ID_CANNOT_BE_READ_WARINING);
                std::cout << "Bad input: line in ContainerAwaitingAtPort input file ignored because container id isn't in the correct size" << std::endl;
                continue;
            }

            if (ship.knowContainerId(containerId)){
                errors.addError(Error::DUPLICATE_CONTAINER_ID_WARINING);
                std::cout << "Warning: container " << containerId << " appears twice" << std::endl;
            }
            else{
                Container newContainer(weight,destinationPort,containerId);
                waitingContainers.push_back(newContainer);
                ship.updateContainerMapping(newContainer);
            }
        }
        planFile.close();
    }
    return errors.getErrorsCode();
}

int IO::readShipPlan(const string &path, ShipPlan& shipPlan) {
    Errors errors;
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
        shipPlan = ShipPlan(plan,numOfFloors);

        while (getline(planFile, line)) {
            errors.addErrors(createPositionFromRowInput(numOfFloors, X, Y, line, shipPlan));
        }
        planFile.close();
    }
    else{
        errors.addError(Error::PLAN_FILE_CANNOT_BE_READ_ERROR);
    }
    return errors.getErrorsCode();
}

int IO::readShipRoute(const string &path, Route& route) {
    Errors errors;
    vector<string> ports;
    int rowIndex = 0;
    string line;
    vector<string> row;

    std::ifstream planFile(path);

    if (planFile.is_open()) {
        while (getline(planFile, line)) {
            row = breakLineToWords(line, ' ');

            if (!checkPortNumberInput(row)) {
                continue;
            }
            std::transform(row[0].begin(), row[0].end(), row[0].begin(), ::toupper);
            if(iscntrl(row[0].at(row[0].size()-1)))
            {
                row[0] = row[0].substr(0, row[0].size()-1);
            }
            if (rowIndex > 0 && ports[rowIndex].compare(ports[rowIndex-1])) {
                errors.addError(Error::PORT_APPEAR_TWICE_WARNING);
            } else{
                ports.push_back(row[0]);
                rowIndex++;
            }
        }
        planFile.close();
        route = Route(ports);
    }
    else{
        errors.addError(Error::ROUTE_FILE_CANNOT_BE_READ_ERROR);
    }
    return errors.getErrorsCode();
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

int IO::writeToFile(const string &writingPath, const string &content) {
    std::ofstream fout;
    fout.open(writingPath, std::fstream::app);
    if (fout.is_open())
    {
        fout << content;
        fout.close();
        return 0;
    }

    std::cout << "Unable to open file";
    return 1;
}

// endregion