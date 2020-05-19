//
// Created by Yarden on 09/05/2020.
//

#ifndef CPP_STOWAGE_ALGORITHMRESULTS_H
#define CPP_STOWAGE_ALGORITHMRESULTS_H

#include <fstream>
#include <map>



class AlgorithmResults
{
private:
    std::string algorithmName;
    int operationsCounterOnAllTravels = 0;
    int numberOfFailedTravels = 0;
    std::map<std::string, int> operationsCounterOnTravel;
public:
    int getNumberOfFailedTravels() const;

public:
    const std::string &getAlgorithmName() const;

    AlgorithmResults();

    AlgorithmResults(const std::string &algorithmName);

    void addTravelResult(const std::string &travelName, int result);

    static bool comparePairs(const std::pair<std::string, AlgorithmResults> &algorithmresult1,
                                        const std::pair<std::string, AlgorithmResults> &algorithmresult2){
        if(algorithmresult1.second.operationsCounterOnAllTravels != algorithmresult2.second.operationsCounterOnAllTravels)
        {
            return algorithmresult1.second.operationsCounterOnAllTravels < algorithmresult2.second.operationsCounterOnAllTravels;
        }
        return algorithmresult1.second.numberOfFailedTravels > algorithmresult2.second.numberOfFailedTravels;
    }

    int getOperationsCounterOnAllTravels() const;

    int getOperationCounterOnOneTravel(const std::string& name);

};

#endif //CPP_STOWAGE_ALGORITHMRESULTS_H
