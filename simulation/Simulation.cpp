//
// Created by Yarden on 03/05/2020.
//


#include "Simulation.h"
#include "../common/utils/Registrar.h"
#include <functional>
#include <memory>
#include "../algorithm/WeightBalanceCalculator.h"
#include "../algorithm/_206039984_a.h"


using namespace std;

Simulation::Simulation(const string &travelsPath, const string &algorithmPath, const string &outputPath): travelsPath(travelsPath), algorithmPath(algorithmPath), outputPath(outputPath)
{
#ifndef RUNNING_ON_NOVA
    Registrar::getInstance().factoryVec.emplace_back([](){return std::make_unique<_206039984_a>();});
    Registrar::getInstance().addName("_206039984_a");
    for(unsigned long long i = 0 ; i < Registrar::getInstance().factoryVec.size(); i++)
    {
        algorithmsResults[Registrar::getInstance().names[i]] = AlgorithmResults(Registrar::getInstance().names[i]);
    }
#endif

    if(algorithmPath == "")
    {
        //load algorithms from cwd
    } else
    {
        //load algorithms from algorithmPath
    }

    if(outputPath!="")
    {
        this->outputPath = outputPath;
        isOutputPathSupplied = true;
    }

    // TODO -------- do it in loop ---------After going to nova
    //Registrar::getInstance().loadSO("<path of x>");
    // TODO ------------- end ---------------


}

int Simulation::simulateAllTravelsWithAllAlgorithms()
{
    if (travelsPath == "")
    {
        cout << "Fatal error: missing travel_path argument!" << endl;
        return 1;
    }

    if(std::filesystem::exists(travelsPath) == 0)
    {
        cout << "Fatal error: travel_path argument isn't valid!" << endl;
        return 1;
    }

    vector <string> travelNames;

    for(auto& travel: std::filesystem::directory_iterator(travelsPath))
    {
        string pathOfCurrentTravel = travel.path().string();
        travelNames.push_back(travel.path().stem().string());
        simulateOneTravelWithAllAlgorithms(pathOfCurrentTravel);
    }

    if(!isOutputPathSupplied)
    {
        outputPath = travelsPath;
    }
    string resultOutputPath = outputPath + "/simulation.results.csv"; //TODO: remove csv;

    IO::writeToFile(resultOutputPath, "RESULTS,");
    for(string travelName : travelNames)
    {
        IO::writeToFile(resultOutputPath, travelName + ",");
    }
    IO::writeToFile(resultOutputPath, "SUM,Num Errors\n");
   //TODO: sort algorithmResults
    for(std::pair<string, AlgorithmResults> algorithmResults : algorithmsResults)
    {
        IO::writeToFile(resultOutputPath, algorithmResults.second.getAlgorithmName() + ",");

        for(string travelName : travelNames)
        {
            int operationCounteOnCurrentTravel = algorithmResults.second.getOperationCounterOnOneTravel(travelName);
            IO::writeToFile(resultOutputPath, std::to_string(operationCounteOnCurrentTravel) + ",");
        }
        IO::writeToFile(resultOutputPath, to_string(algorithmResults.second.getOperationsCounterOnAllTravels()) + ",");
        IO::writeToFile(resultOutputPath, to_string(algorithmResults.second.getNumberOfFailedTravels()) + "\n");
    }

    return 0;
}

void Simulation::simulateOneTravelWithAllAlgorithms(const string &travelPath) {

    cout << "Started simulate " << travelPath << endl;
    if(isOutputPathSupplied == false)
    {
        this->outputPath = travelPath;
    }

    const std::filesystem::path path = travelPath;

    if(is_directory(path) == 0) //travelPath isn't a directory
    {
        cout << "Travel error: " << travelPath << " is not a directory" << endl;
        return;
    }

    std::ofstream file { travelPath + "/empty_containers_awaiting_at_port_file.txt" };

    Registrar &registrar = Registrar::getInstance();

    for(size_t i = 0 ; i < registrar.factoryVec.size() ; i++ )
    {
        unique_ptr<AbstractAlgorithm> algorithm = registrar.factoryVec[i]();
        simulateOneTravelWithOneAlgorithm(travelPath, std::move(algorithm), registrar.names[i]);
    }



    cout << "Finished simulate " << travelPath << endl;

}

void Simulation::simulateOneTravelWithOneAlgorithm(const string &travelPath, unique_ptr<AbstractAlgorithm> algorithm, const string &algorithmName) {

    std::cout <<"===========================================================" << std::endl;
    std::cout <<"started simulate " << algorithmName << " on " << travelPath << std::endl;
    std::cout <<"===========================================================" << std::endl;


    int totalOperations = 0;
    Route route;
    vector<string> ports;
    Ship ship;
    CraneManagement craneManagement;
    CraneManagement::CraneManagementAnswer craneManagementAnswer;
    WeightBalanceCalculator weightBalanceCalculator;
    algorithm->setWeightBalanceCalculator(weightBalanceCalculator);
    std::map<string,int> indexOfVisitAtPort;
    std::map<string,int> totalNumbersOfVisitingPort;

    const std::filesystem::path pathOfTravel = travelPath;
    const string travelName = pathOfTravel.stem().string();
    const std::filesystem::path pathOfOutputFilesForAlgorithmAndTravel = outputPath + "/output_of_" + algorithmName + "_" + travelName; //TODO: replace algorithm name

    std::filesystem::create_directory(pathOfOutputFilesForAlgorithmAndTravel);

    cout << "Reading route" << endl;
    IO::readShipRoute(travelPath + "/route.txt", route);
    ports = route.getPorts();
    for(string port : ports)
    {
        totalNumbersOfVisitingPort[port]++;
    }
    algorithm->readShipRoute(travelPath + "/route.txt");

    cout << "Reading ship plan" << endl;
    IO::readShipPlan(travelPath + "/ship_plan.txt", ship.getShipPlan());
    algorithm->readShipPlan(travelPath + "/ship_plan.txt");

    for (size_t i = 0; i < ports.size(); i++)
    {
        std::cout << "----------------" <<"Ship Arrived to port " << ports[i] << "----------------" << std::endl;
        const std::filesystem::path pathOfContainersAwaitingAtPortFile = travelPath + "/" + ports[i] + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".cargo_data.txt"; //TODO: remove .txt in linux
        if(std::filesystem::exists(pathOfContainersAwaitingAtPortFile) == 0)
        {
            std::cout << "No containers awaiting at port input file for " << ports[i] << " for visiting number " << indexOfVisitAtPort[ports[i]] + 1 << std::endl;
            algorithm->getInstructionsForCargo(travelPath + "empty_containers_awaiting_at_port_file.txt", pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + "algorithm_name" + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".txt");
        }
        else
        {
            algorithm->getInstructionsForCargo(travelPath + "/" + ports[i] + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".cargo_data.txt", pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + "algorithm_name" + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".txt"); //TODO: remove .txt in linux and correctly get algorithm name
        }

        craneManagementAnswer = craneManagement.readAndExecuteInstructions(ship, pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + "algorithm_name" + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".txt");
        totalOperations += craneManagementAnswer.numOfOperations;
        checkForErrorsAfterPort(ship, ports[i], craneManagementAnswer, route, pathOfOutputFilesForAlgorithmAndTravel.string());
        indexOfVisitAtPort[ports[i]]++;
        std::cout << "----------------" <<"Ship left port " << ports[i] << "----------------" << std::endl;
        route.incrementCurrentPort();

    }
    algorithmsResults[algorithmName].addTravelResult(travelName, totalOperations);

    std::cout <<"===========================================================" << std::endl;
    std::cout <<"finished simulate " << algorithmName << " on " << travelPath << std::endl;
    std::cout <<"===========================================================" << std::endl;

}

void Simulation::checkForErrorsAfterPort(Ship& ship, const string &port, CraneManagement::CraneManagementAnswer& answer,Route& route, const string &pathOfOutputFilesForAlgorithmAndTravel) {

        if (ship.portToContainers[port].size() > 0)
        {
            string error = "Not all of the containers of this port were unloaded";
            cout << error;
            IO::writeToFile(pathOfOutputFilesForAlgorithmAndTravel, error + ",");
        }

        if (answer.changedContainers.count(Action::REJECT) > 0)
        {
            for (auto& rejected : answer.changedContainers[Action::REJECT])
            {
                for (auto &loaded : answer.changedContainers[Action::LOAD])
                {
                    if (route.nextStopForPort(ship.containerIdToDestination(rejected)) <
                        route.nextStopForPort(ship.containerIdToDestination(loaded)))
                    {
                        string error = "Rejected container " + rejected + " with higher priority";
                        cout << error;
                        IO::writeToFile(pathOfOutputFilesForAlgorithmAndTravel, error + ",");
                    }
                }
            }
        }

        if (route.inLastStop())
        {
            int amountOfContainers = ship.getAmountOfContainers() > 0;
            if (amountOfContainers>0)
            {
                string error = "Ship arrived to last port in route but there are still " + std::to_string(amountOfContainers) + " containers on the ship";
                cout << error;
                IO::writeToFile(pathOfOutputFilesForAlgorithmAndTravel, error + ",");
            }
        }

}