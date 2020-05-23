//
// Created by Yarden on 09/05/2020.
//

#include "AlgorithmResults.h"

AlgorithmResults::AlgorithmResults(const std::string &algorithmName) : algorithmName(algorithmName) {}

void AlgorithmResults::addTravelResult(const std::string &travelName, int result) {
    operationsCounterOnTravel[travelName] = result;
    if(result != -1)
    {
        operationsCounterOnAllTravels += result;
    } else
    {
        numberOfFailedTravels++;
    }
}

AlgorithmResults::AlgorithmResults() {}

int AlgorithmResults::getOperationsCounterOnAllTravels() const {
    return operationsCounterOnAllTravels;
}

const std::string &AlgorithmResults::getAlgorithmName() const {
    return algorithmName;
}

int AlgorithmResults::getOperationCounterOnOneTravel(const std::string& name)
{
    return operationsCounterOnTravel[name];
}

int AlgorithmResults::getNumberOfFailedTravels() const {
    return numberOfFailedTravels;
}
