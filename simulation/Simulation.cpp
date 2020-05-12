//
// Created by Yarden on 03/05/2020.
//


#include "Simulation.h"
#include "../common/utils/Registrar.h"
#include <functional>
#include <memory>

using namespace std;

Simulation::Simulation(const string &travelsPath, const string &algorithmPath, const string &outputPath): travelsPath(travelsPath), algorithmPath(algorithmPath), outputPath(outputPath)
{
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

    // TODO -------- do it in loop ---------
    Registrar::getInstance().loadSO("<path of x>"); // relavant only to nova
    unique_ptr<AbstractAlgorithm> x = Registrar::getInstance().factoryVec[0]();
    string x_name = Registrar::getInstance().names[0];
    // TODO ------------- end ---------------


    NaiveAlgorithm *naiveAlgorithm1 = new NaiveAlgorithm(); //TODO: remove after testing and load from file
    algorithms.push_back(naiveAlgorithm1);


    for(AbstractAlgorithm* algorithm : algorithms)
    {
        algorithmResults["algorithm_name"] = AlgorithmResults("algorithm_name");
        if(algorithm!= nullptr) //TODO: the if is just for "unused parameter error, reomve it
        {
            cout << "just for testing" << endl;
        }
    }
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

    for(auto& travel: std::filesystem::directory_iterator(travelsPath))
    {
        string pathOfCurrentTravel = travel.path().string();
        simulateOneTravelWithAllAlgorithms(pathOfCurrentTravel);
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

    for(AbstractAlgorithm* algorithm : algorithms)
    {
        simulateOneTravelWithOneAlgorithm(travelPath, algorithm);
    }


    cout << "Finished simulate " << travelPath << endl;

}

void Simulation::simulateOneTravelWithOneAlgorithm(const string &travelPath, AbstractAlgorithm *algorithm) {

    std::cout <<"===========================================================" << std::endl;
    std::cout <<"started simulate " << "algorithm_name" << " on " << travelPath << std::endl; //TODO: get algorithm name correctly
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
    const std::filesystem::path pathOfOutputFilesForAlgorithmAndTravel = outputPath + "/output_of_" + "algorithm_name" + "_" + travelName; //TODO: replace algorithm name

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

        craneManagementAnswer = craneManagement.readAndExecuteInstructions(&ship, pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + "algorithm_name" + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".txt");
        totalOperations += craneManagementAnswer.numOfOperations;
        checkForErrorsAfterPort(ship, ports[i], craneManagementAnswer, route);
        indexOfVisitAtPort[ports[i]]++;
        std::cout << "----------------" <<"Ship left port " << ports[i] << "----------------" << std::endl;
        route.incrementCurrentPort();

    }
    algorithmResults["algorithm_name"].addTravelResult(travelPath, totalOperations);

    std::cout <<"===========================================================" << std::endl;
    std::cout <<"finished simulate " << "algorithm_name" << " on " << travelPath << std::endl; //TODO: get algorithm name correctly
    std::cout <<"===========================================================" << std::endl;

}

void Simulation::checkForErrorsAfterPort(Ship& ship, const string &port, CraneManagement::CraneManagementAnswer& answer,Route& route) {

        if (ship.portToContainers[port].size() > 0){
            // "Not all of the containers of this port were unloaded,";
        }

        if (answer.changedContainers.count(Action::REJECT) > 0) {
            for (auto& rejected : answer.changedContainers[Action::REJECT]) {
                for (auto &loaded : answer.changedContainers[Action::LOAD]) {
                    if (route.nextStopForPort(ship.containerIdToDestination(rejected)) <
                        route.nextStopForPort(ship.containerIdToDestination(loaded))) {
                        //"Rejected container " << rejected << " with higher priority,";
                    }
                }
            }
        }
        if (route.inLastStop()) {
            int amountOfContainers = ship.getAmountOfContainers() > 0;
            if (amountOfContainers>0){
                // "Ship arrived to last port in route but there are still " << amountOfContainers << " containers on the ship,";
            }
        }

}