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
    int getNumberOfFailedTravels() const;

public:
    const string &getAlgorithmName() const;

    AlgorithmResults();

    AlgorithmResults(const string &algorithmName);

    void addTravelResult(const string &travelName, int result);

    static bool comparePairs(const std::pair<string, AlgorithmResults> &algorithmresult1,
                                        const std::pair<string, AlgorithmResults> &algorithmresult2){
        if(algorithmresult1.second.operationsCounterOnAllTravels != algorithmresult2.second.operationsCounterOnAllTravels)
        {
            return algorithmresult1.second.operationsCounterOnAllTravels < algorithmresult2.second.operationsCounterOnAllTravels;
        }
        return algorithmresult1.second.numberOfFailedTravels > algorithmresult2.second.numberOfFailedTravels;
    }

    int getOperationsCounterOnAllTravels() const;

    int getOperationCounterOnOneTravel(const string& name);

};

#endif //CPP_STOWAGE_ALGORITHMRESULTS_H
