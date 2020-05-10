//
// Created by Yarden on 09/05/2020.
//

#ifndef CPP_STOWAGE_ALGORITHMRESULTS_H
#define CPP_STOWAGE_ALGORITHMRESULTS_H

#include <fstream>
#include <map>
using namespace std;



class AlgorithmResults
{
private:
    string algorithmName;
    int operationsCounterOnAllTravels = 0;
    int numberOfFailedTravels = 0;
    map<string, int> operationsCounterOnTravel;

public:
    AlgorithmResults();

    AlgorithmResults(const string &algorithmName);

    void addTravelResult(const string &travelName, int result);


};

#endif //CPP_STOWAGE_ALGORITHMRESULTS_H
