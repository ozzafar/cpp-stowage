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

int IO::createPositionFromRowInput(int numOfFloors, int X, int Y, string &line, ShipPlan& shipPlan,Errors& errors) {
    std::map <std::pair<int, int>, int> updatedCoordinates;
    int x, y, actualNumOfFloors;
    vector<string> row = IO::breakLineToWords(line, ',');
    if(row.size() < 3 || !isNumber(row[0]) || !isNumber(row[1]) || !isNumber(row[2]))
    {
        errors.addError(Error::PLAN_BAD_LINE_FORMAT_WARNING,line);
        std::cout << "\t\tWarning: bad input" << std::endl;
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
                errors.addError(Error::PLAN_BAD_LINE_FORMAT_WARNING,line);
            }
            else
            {
                errors.addError(Error::DUPLICATE_X_Y_WITH_DIFFERENT_DATA,line);
                return errors.getErrorsCode();
            }
        }
        else if (actualNumOfFloors > numOfFloors) {
            errors.addError(Error::NUMBER_OF_FLOORS_WARNING,line);
            std::cout << "\t\tWarning: in position (" << x << "," << y << ") the actual number of floors: "
                      << actualNumOfFloors << " is illegal (max number is " << numOfFloors << ")"
                      << std::endl;
        }
        else if (actualNumOfFloors == numOfFloors) {
            errors.addError(Error::NUMBER_OF_FLOORS_WARNING,line);
            std::cout << "\t\tWarning: in position (" << x << "," << y << ") the actual number of floors: "
                      << actualNumOfFloors << " is the maximum number (" << numOfFloors << ")"
                      << std::endl;
        } else if (x < 0 || x >= X || y < 0 || y >= Y) {
            std::cout << "\t\tWarning: the position (" << x << "," << y << ") is illegal" << std::endl;
            errors.addError(Error::ILLEGAL_POSITION_WARNING,line);
        } else {
            shipPlan.setStartFloorInPosition(x, y, numOfFloors - actualNumOfFloors);
        }
    }
    return errors.getErrorsCode();
}

static bool shouldSkipLine(vector<string> portNumber)
{
    if (portNumber.empty() == 1) {
        std::cout << "\t\tline in route input file ignored because it is an empty line" << std::endl;
        return true;
    }

    if (portNumber[0].at(0) == '#') {
        std::cout << "\t\tline in route input file ignored because it is a comment" << std::endl;
        return true;
    }
    return false;
}


static int checkPortNumberInput(vector<string> portNumber,Errors& errors) {
    if (portNumber.size() != 1) {
        std::cout << "\t\tBad input: line in route input file ignored because it didn't contain one word" << std::endl;
        errors.addError(Error::BAD_PORT_SYMBOL_WARNING);
        return -1;
    }

    if(iscntrl(portNumber[0].at(portNumber[0].size()-1)))
    {
        portNumber[0] = portNumber[0].substr(0, portNumber[0].size()-1);
    }

    if (portNumber[0].size() != 5) {
        std::cout << "\t\tline in route input file ignored because it has no 5 chars" << std::endl;
        errors.addError(Error::BAD_PORT_SYMBOL_WARNING,portNumber[0]);
        return -1;
    }

    if (!isAlphabetString(portNumber[0])) {
        std::cout << "\t\tline in route input file ignored because it contains non alphabet chars" << std::endl;
        errors.addError(Error::BAD_PORT_SYMBOL_WARNING,portNumber[0]);
        return -1;
    }
    return 0;
}

// endregion

// region MAIN FUNCTIONS

int IO::readContainerAwaitingAtPortFile(const string &input_path, Ship& ship, vector<Container>& waitingContainers, vector<Container>& badContainers,Errors& errors) {
    string line;
    std::ifstream planFile(input_path);
    vector<string> row;
    std::set <string> updatedContainersId;

    if (planFile.is_open()) {
        while (getline(planFile, line)) {
            bool badContainer = false;
            row = breakLineToWords(line,',');
            if(row.empty() || row[0].at(0) == '#')
            {
                continue;
            }
            string containerId = row[0];
            if(std::iscntrl(containerId.at(0)))
            {
                errors.addError(Error::ID_CANNOT_BE_READ_WARINING,containerId);
                std::cout << "\t\tBad input: cannot read container ID " << containerId << std::endl;
                continue;
            }
            if (!Container::isValidId(containerId)){
                errors.addError(Error::ILLEGAL_ID_CHECK_WARNING,containerId);
                std::cout << "\t\tBad input: invalid container id " << containerId << std::endl;
                badContainer = true;
            }
            if (updatedContainersId.find(containerId) != updatedContainersId.end())
            {
                errors.addError(Error:: DUPLICATE_CONTAINER_ID_WARINING,containerId);
                std::cout << "\t\tBad input: port already provide this container id " << containerId << std::endl;
                badContainer = true;
            }
            if (ship.knowContainerId(containerId)){
                errors.addError(Error::CONTAINER_ID_ALREADY_IN_SHIP_WARINING,containerId);
                std::cout << "\t\tWarning: container " << containerId << " appears twice" << std::endl;
                badContainer = true;
            }

            if (row.size() < 2){
                errors.addError(Error::MISSING_OR_BAD_WEIGHT_WARINING,line);
                badContainer = true;
            }
            int weight = stoi(row[1]);
            if (weight < 0)
            {
                errors.addError(Error::MISSING_OR_BAD_WEIGHT_WARINING,line);
                badContainer = true;
            }

            if (row.size() < 3){
                errors.addError(Error::MISSING_OR_BAD_DEST_WARINING,line);
                badContainer = true;
            }

            string destinationPort = row[2];
            if(iscntrl(destinationPort.at(destinationPort.size()-1)))
            {
                destinationPort = destinationPort.substr(0, destinationPort.size()-1);
            }
            if (!isAlphabetString(destinationPort)){
                errors.addError(Error::MISSING_OR_BAD_DEST_WARINING,line);
                std::cout << "\t\tBad input: line in ContainerAwaitingAtPort input file ignored because container id contains non alphabet chars" << std::endl;
                badContainer = true;
            }
            if (destinationPort.size() != 5) {
                errors.addError(Error::MISSING_OR_BAD_DEST_WARINING,line);
                std::cout << "\t\tBad input: line in ContainerAwaitingAtPort input file ignored because container id isn't in the correct size" << std::endl;
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
        errors.addError(Error::CONTAINER_FILE_CANNOT_BE_READ_WARNING,input_path);
        std::cout << "\t\tContainer file cannot be read" << std::endl;
    }
    return errors.getErrorsCode();
}

int IO::readShipPlan(const string &path, ShipPlan& shipPlan,Errors& errors) {
    int numOfFloors, X, Y;
    string line, word;

    vector<string> row;

    std::ifstream shipPlanFile(path);
    if (shipPlanFile.is_open()) {
        getline(shipPlanFile, line);
        row = breakLineToWords(line, ',');

        while(row.empty() || row[0].at(0) == '#')
        {
            getline(shipPlanFile, line);
            row = breakLineToWords(line, ',');
        }

        if(row.size() < 3 || !isNumber(row[0]) || !isNumber(row[1]) || !isNumber(row[2]))
        {
            errors.addError(Error::PLAN_FILE_CANNOT_BE_READ_ERROR);
            return errors.getErrorsCode();
        }

        numOfFloors = stoi(row[0]);
        X = stoi(row[1]);
        Y = stoi(row[2]);
        vector<vector<ContainersPosition>> plan(X, vector<ContainersPosition>(Y, ContainersPosition(numOfFloors)));
        shipPlan = ShipPlan(plan,numOfFloors);

        while (getline(shipPlanFile, line)) {
            createPositionFromRowInput(numOfFloors, X, Y, line, shipPlan, errors);
        }
        shipPlanFile.close();
    }
    else{
        errors.addError(Error::PLAN_FILE_CANNOT_BE_READ_ERROR);
    }
    return errors.getErrorsCode();
}

int IO::readShipRoute(const string &path, Route& route,Errors& errors) {
    vector<string> ports;
    int prevPortIndex = -1;
    string line;
    vector<string> row;

    std::ifstream routeFile(path);

    if (routeFile.is_open()) {
        while (getline(routeFile, line)) {
            row = breakLineToWords(line, ' ');

            if(shouldSkipLine(row) == 1)
            {
                continue;
            }

            if (checkPortNumberInput(row,errors) != (int) Error::SUCCESS)
            {
                continue;
            }

            std::transform(row[0].begin(), row[0].end(), row[0].begin(), ::toupper);
            if(iscntrl(row[0].at(row[0].size()-1)))
            {
                row[0] = row[0].substr(0, row[0].size()-1);
            }
            if (prevPortIndex >= 0 && !row[0].compare(ports[prevPortIndex])) {
                errors.addError(Error::PORT_APPEAR_TWICE_WARNING,row[0]);
            } else{
                ports.push_back(row[0]);
                prevPortIndex++;
            }
        }
        routeFile.close();
        if(ports.size() == 0)
        {
            errors.addError(Error::ROUTE_FILE_CANNOT_BE_READ_ERROR);
            return errors.getErrorsCode();
        }
        if(ports.size() == 1)
        {
            errors.addError(Error::ROUTE_FILE_SINGLE_VALID_PORT_ERROR);
            return errors.getErrorsCode();
        }
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
        IO::trim(word);
        row.push_back(word);
    }
    return row;
}

int IO::writeToFile(const string &writingPath, const string &content) {
    // TODO dont open file every call to this function
    std::ofstream fout;
    fout.open(writingPath, std::fstream::app);
    if (fout.is_open())
    {
        fout << content;
        fout.close();
        return 0;
    }

    std::cout << "\t\tUnable to open file" << std::endl;
    return 1;
}

void IO::writeResultsOfSimulation(const string &resultOutputPath, const vector<string> &travelNames,
                                  map<string, AlgorithmResults> &algorithmsResults){
    auto cmp = [](AlgorithmResults const & algorithmResult1, AlgorithmResults const & algorithmResult2)
    {
        if(algorithmResult1.getNumberOfFailedTravels() != algorithmResult2.getNumberOfFailedTravels())
        {
            return algorithmResult1.getNumberOfFailedTravels() < algorithmResult2.getNumberOfFailedTravels();
        }
        return algorithmResult1.getOperationsCounterOnAllTravels() < algorithmResult2.getOperationsCounterOnAllTravels();
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

void IO::writeErrorsOfTravelAndAlgorithm (Errors &simErrors,Errors &algErrors, const string &outputPathOfErrorsFile) {
    IO::writeToFile(outputPathOfErrorsFile, "///////////////////// SIMULATION /////////////////////\n///////////////////// ERRORS     /////////////////////\n\n");
    auto simItr = simErrors.getIterator();
    while(simItr.hasNext())
    {
        Error error = simItr.getNext();
        IO::writeToFile(outputPathOfErrorsFile, simErrors.errorToString(error) + '\n');
    }
    IO::writeToFile(outputPathOfErrorsFile, "///////////////////// ALGORITHM /////////////////////\n///////////////////// ERRORS    /////////////////////\n\n");
    auto algItr = algErrors.getIterator();
    while(algItr.hasNext())
    {
        Error error = algItr.getNext();
        IO::writeToFile(outputPathOfErrorsFile, algErrors.errorFromAlgorithmToString(error) + '\n');
    }
}

bool IO::isNumber(const std::string &s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

string IO::firstFileWithExtensionInDirectory(const string& pathOfDirectory, const string &extension) {
    string pathOfFirstFileWithExtension;

    for(auto& file: std::filesystem::directory_iterator(pathOfDirectory))
    {
        string pathOfCurrentTravel = file.path().string();
        if(pathOfCurrentTravel.substr(pathOfCurrentTravel.find_last_of(".") + 1) == extension)
        {
            pathOfFirstFileWithExtension = pathOfCurrentTravel;
            break;
        }
    }
    return pathOfFirstFileWithExtension;
}

void IO::clearPreviousOutput(const string &outputPath) {
    if(std::filesystem::exists(outputPath) == 1)
    {
        for(auto& dir: std::filesystem::directory_iterator(outputPath))
        {
            string fileName = dir.path().string();
            if (fileName.find("crane_instructions") != string::npos || fileName.find("errors") != string::npos
                || fileName.find("results") != string::npos)
            {
                std::filesystem::remove_all(fileName);
            }
        }
    }

}


// trim from start (in place)
void IO::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void IO::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void IO::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}





// endregion