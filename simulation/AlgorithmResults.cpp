//
// Created by Yarden on 09/05/2020.
//

#include "AlgorithmResults.h"

AlgorithmResults::AlgorithmResults(const string &algorithmName) : algorithmName(algorithmName) {}

void AlgorithmResults::addTravelResult(const string &travelName, int result) {
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
