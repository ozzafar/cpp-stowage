//
// Created by Oz Zafar on 09/04/2020.
//

#include "Algorithm.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "ContainersPosition.h"
#include "CraneOperation.h"
#include <list>
#include <string>
#include <algorithm>

using std::stringstream;
using std::string;
using std::list;

void Algorithm::readShipPlan(const string &path) {
    int numOfFloors, X, Y;
    string line,word;
    std::ifstream planFile(path);
    vector<string> row;

    getline(planFile, line);
    breakLineToWords(line, row, ',');
    numOfFloors = stoi(row[0]);
    X = stoi(row[1]);
    Y = stoi(row[2]);
    vector<vector<ContainersPosition>> plan(X, vector<ContainersPosition>(Y,ContainersPosition()));
    shipPlan = new ShipPlan(numOfFloors,plan);

    int x, y, actualNumOfFloors;
    if (planFile.is_open()) {
        while (getline(planFile, line)) {
            breakLineToWords(line, row, ',');
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

void Algorithm::breakLineToWords(string &line, vector<string> &row, char delimeter) {
    string word;
    row.clear();
    stringstream ss(line);
    ws(ss);
    while (getline(ss, word, delimeter)){
        row.push_back(word);
    }
}

void print(std::list<std::string> const &list)
{
    for (auto const& i: list) {
        std::cout << i << "\n";
    }
}

bool isAlphabetString(const string& str)
{
    char ch;
    for(char i : str)
    {
        ch = i;
        if((ch < 65) || (ch > 122) || (ch > 90 && ch < 97))
        {
            return false;
        }
    }
    return true;
}

bool checkPortNumberInput(vector<string> portNumber)
{
    if(portNumber.empty() == 1)
    {
        std::cout << "line in route input file ignored because it is an empty line" << std::endl;
        return false;
    }

    if(portNumber[0].at(0)=='#')
    {
        std::cout << "line in route input file ignored because it is a comment" << std::endl;
        return false;
    }

    if(portNumber.size()!=1)
    {
        std::cout << "Bad input: line in route input file ignored because it didn't contain one word" << std::endl;
        return false;
    }

    if(portNumber[0].size()!=5)
    {
        std::cout << "Bad input: line in route input file ignored because it has no 5 chars" << std::endl;
        return false;
    }

    if(!isAlphabetString(portNumber[0]))
    {
        std::cout << "Bad input: line in route input file ignored because it contains non alphabet chars" << std::endl;
        return false;
    }

    return true;

}

void Algorithm::readShipRoute(const string &path) {
    list<string> ports;
    int rowIndex=0;
    string line;
    vector<string> row;

    std::ifstream planFile(path);

    if (planFile.is_open()) {
        while (getline(planFile, line)) {
            rowIndex++;
            std::cout << "Reading input route file, line " << rowIndex << std::endl;
            breakLineToWords(line, row, ' ');

            if (!checkPortNumberInput(row)) {
                continue;
            }

            std::transform(row[0].begin(), row[0].end(), row[0].begin(), ::toupper);
            ports.push_back(row[0]);
        }
        planFile.close();
        print(ports);

    }
    shipRoute = *new Route(ports);

}

ShipPlan* Algorithm::getShipPlan() {
    return shipPlan;
}


// input_path is Containers-awaiting-at-port file
void Algorithm::getInstructionsForCargo(const string &input_path, const string &output_path) {

    //---------------------- START UNLOAD -------------------------
    //TODO unload all containers in destination
    //---------------------- END UNLOAD -------------------------

    vector<Container*> containers = readContainerAwaitingAtPortFile(input_path);
    updateContainerMapping(containers);
    // ---------------------- START LOAD -------------------------
    int amount = containers.size(), index = 0;
    for (int i = 0; i < shipPlan->getPlanLength(); ++i) {
        for (int j = 0; j < shipPlan->getPlanWidth(); ++j) {
            ContainersPosition containersPosition = shipPlan->getContainerPosition(i, j);
            for (int k = 0; k < containersPosition.getNumOfActiveFloors(); k++) {
                if (index < amount) {
                    if (calculator.tryOperation((char) CraneOperation::LOAD, containers.at(index)->getWeight(), i, j)) {
                        index++;
                        writeOperation(output_path,CraneOperation::LOAD,containers[index]->getId(),containersPosition.getTopFloorNumber(),i,j);
                    }
                } else {
                    return;
                }
            }
        }
    }
    if (index < amount){
        for (; index < amount ; index++){
            writeOperation(output_path,CraneOperation::REJECT,containers[index]->getId(),-1,-1,-1);
        }
    }
    // ---------------------- END LOAD -------------------------

}

vector<Container*> Algorithm::readContainerAwaitingAtPortFile(const string &path) {
    string line;
    std::ifstream planFile(path);
    vector<string> row;
    vector<Container*> containers;

    if (planFile.is_open()) {
        while (getline(planFile, line)) {
            breakLineToWords(line, row,',');
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

void Algorithm::writeOperation(const std::string& filename, CraneOperation op, const string& containerId, int floor, int x, int y)
{
    std::ofstream fout;
    fout.open(filename);
    if (fout.is_open())
    {
        fout << (char)op << "," << containerId << "," << floor << "," << x << "," << y << '\n';
        fout.close();
    }
    else {
        std::cout << "Unable to open file";
    }
}
