//
// Created by Oz Zafar on 18/04/2020.
//


#include "IO.h"


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
    std::map <std::pair<int, int>, int> updatedCoordinates;
    Errors errors;
    int x, y, actualNumOfFloors;
    vector<string> row = IO::breakLineToWords(line, ',');
    if(row.size() < 3 || !IO::isNumber(row[0]) || !IO::isNumber(row[1]) || !IO::isNumber(row[2]))
    {
        errors.addError(Error::PLAN_BAD_LINE_FORMAT_WARNING);
        std::cout << "Warning: bad input" << std::endl;
    }
    else
        {
        x = stoi(row[0]);
        y = stoi(row[1]);
        actualNumOfFloors = stoi(row[2]);
        if(updatedCoordinates.find(std::pair(x,y)) != updatedCoordinates.end())
        {
            if(updatedCoordinates[std::pair(x,y)] == actualNumOfFloors)
            {
                errors.addError(Error::PLAN_BAD_LINE_FORMAT_WARNING);
            }
            else
            {
                errors.addError(Error::DUPLICATE_X_Y_WITH_DIFFERENT_DATA);
                return errors.getErrorsCode();
            }
        }
        else if (actualNumOfFloors > numOfFloors) {
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
            errors.addError(Error::ILLEGAL_POSITION_WARNING);
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

int IO::readContainerAwaitingAtPortFile(const string &input_path, Ship& ship, vector<Container>& waitingContainers, vector<Container>& badContainers) {
    //TODO: check for error: total containers amount exceeds ship capacity (rejecting far containers)
    Errors errors;
    string line;
    std::ifstream planFile(input_path);
    vector<string> row;
    std::set <string> updatedContainersId;

    if (planFile.is_open()) {
        while (getline(planFile, line)) {
            bool badContainer = false;
            row = breakLineToWords(line,',');
            if(row.empty())
            {
                continue;
            }
            string containerId = row[0];
            if(std::iscntrl(containerId.at(0)))
            {
                errors.addError(Error::ID_CANNOT_BE_READ_WARINING);
                std::cout << "Bad input: cannot read container ID " << containerId << std::endl;
                continue;
            }
            if (!Container::isValidId(containerId)){
                errors.addError(Error::ILLEGAL_ID_CHECK_WARNING);
                std::cout << "Bad input: invalid container id " << containerId << std::endl;
                badContainer = true;
            }
            if (updatedContainersId.find(containerId) != updatedContainersId.end())
            {
                errors.addError(Error:: DUPLICATE_CONTAINER_ID_WARINING);
                std::cout << "Bad input: port already provide this container id " << containerId << std::endl;
                badContainer = true;
            }
            if (ship.knowContainerId(containerId)){
                errors.addError(Error::CONTAINER_ID_ALREADY_IN_SHIP_WARINING);
                std::cout << "Warning: container " << containerId << " appears twice" << std::endl;
                badContainer = true;
            }

            if (row.size() < 2){
                errors.addError(Error::MISSING_OR_BAD_WEIGHT_WARINING);
                badContainer = true;
            }
            int weight = stoi(row[1]);
            if (weight < 0)
            {
                errors.addError(Error::MISSING_OR_BAD_WEIGHT_WARINING);
                badContainer = true;
            }

            if (row.size() < 3){
                errors.addError(Error::MISSING_OR_BAD_DEST_WARINING);
                badContainer = true;
            }

            string destinationPort = row[2];
            if(iscntrl(destinationPort.at(destinationPort.size()-1)))
            {
                destinationPort = destinationPort.substr(0, destinationPort.size()-1);
            }
            if (!isAlphabetString(destinationPort)){
                errors.addError(Error::MISSING_OR_BAD_WEIGHT_WARINING);
                std::cout << "Bad input: line in ContainerAwaitingAtPort input file ignored because container id contains non alphabet chars" << std::endl;
                badContainer = true;
            }
            if (destinationPort.size() != 5) {
                errors.addError(Error::MISSING_OR_BAD_WEIGHT_WARINING);
                std::cout << "Bad input: line in ContainerAwaitingAtPort input file ignored because container id isn't in the correct size" << std::endl;
                badContainer = true;
            }

            Container newContainer(weight,destinationPort,containerId);
            if (badContainer){
                badContainers.push_back(newContainer);
            }
            else {
                updatedContainersId.insert(containerId);
                ship.updateContainerMapping(newContainer);
                waitingContainers.push_back(newContainer);
            }
        }
        planFile.close();
    }
    else
    {
        errors.addError(Error::CONTAINER_FILE_CANNOT_BE_READ_WARNING);
        std::cout << "Container file cannot be read" << std::endl;
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

        if(row.size() < 3 || !isNumber(row[0]) || !isNumber(row[1])) //TODO: don't know why can't check row[2] the same...maybe ctrl char at end
        {
            errors.addError(Error::PLAN_FILE_CANNOT_BE_READ_ERROR);
            return errors.getErrorsCode();
        }

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
    //TODO: check for error: travel route: bad port symbol format (ignored)
    //TODO: check for error: travel route: travel error - file with only a single valid port (cannot run this travel)
    Errors errors;
    vector<string> ports;
    int prevPortIndex = -1;
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
            if (prevPortIndex >= 0 && !row[0].compare(ports[prevPortIndex])) {
                errors.addError(Error::PORT_APPEAR_TWICE_WARNING);
            } else{
                ports.push_back(row[0]);
                prevPortIndex++;
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

void IO::writeResultsOfsimulation(const string &resultOutputPath, const vector<string> &travelNames,
                                  map<string, AlgorithmResults> &algorithmsResults){
    auto cmp = [](AlgorithmResults const & algorithmResult1, AlgorithmResults const & algorithmResult2)
    {
        if(algorithmResult1.getNumberOfFailedTravels() != algorithmResult2.getNumberOfFailedTravels())
        {
            return algorithmResult1.getNumberOfFailedTravels() > algorithmResult2.getNumberOfFailedTravels();
        }
        return algorithmResult1.getOperationsCounterOnAllTravels() > algorithmResult2.getOperationsCounterOnAllTravels();
    };

    std::vector<AlgorithmResults> algorithmResultsVector;
    for(map<std::string, AlgorithmResults>::iterator it = algorithmsResults.begin(); it != algorithmsResults.end(); ++it )
    {
        algorithmResultsVector.push_back( it->second );
    }

    std::sort(algorithmResultsVector.begin(), algorithmResultsVector.end(), cmp);

    writeToFile(resultOutputPath, "RESULTS,");
    for(string travelName : travelNames)
    {
        writeToFile(resultOutputPath, travelName + ",");
    }
    writeToFile(resultOutputPath, "SUM,Num Errors\n");

    for(AlgorithmResults algorithmResults : algorithmResultsVector)
    {
        writeToFile(resultOutputPath, algorithmResults.getAlgorithmName() + ",");

        for(string travelName : travelNames)
        {
            int operationCounteOnCurrentTravel = algorithmResults.getOperationCounterOnOneTravel(travelName);
            writeToFile(resultOutputPath, std::to_string(operationCounteOnCurrentTravel) + ",");
        }
        writeToFile(resultOutputPath, std::to_string(algorithmResults.getOperationsCounterOnAllTravels()) + ",");
        writeToFile(resultOutputPath, std::to_string(algorithmResults.getNumberOfFailedTravels()) + "\n");
    }


}

void IO::writeErrorsOfTravelAndAlgorithm (Errors &errors, const string &outputPathOfErrorsFile) {
    auto itr = errors.getIterator();
    while(itr.hasNext())
    {
        Error error = itr.getNext();
        IO::writeToFile(outputPathOfErrorsFile, Errors::errorToString(error) + ",");
    }

}

bool IO::isNumber(const std::string &s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

// endregion