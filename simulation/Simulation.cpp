//
// Created by Yarden on 03/05/2020.
//


#include "Simulation.h"
#include "../common/utils/Registrar.h"
#include <functional>
#include <memory>
#include "../algorithm/WeightBalanceCalculator.h"
#include "../algorithm/_206039984_a.h"

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
    if (checkTravelsPath(travelsPath)!=0)
    {
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

    //TODO: sort algorithmResults
    string resultOutputPath = outputPath + "/simulation.results.csv"; //TODO: remove csv;
    IO::writeResultsOfsimulation(resultOutputPath, travelNames, algorithmsResults);

    return 0;
}

void Simulation::simulateOneTravelWithAllAlgorithms(const string &travelPath) {

    cout << "Started simulate " << travelPath << endl;

    const std::filesystem::path path = travelPath;
    if(is_directory(path) == 0) //travelPath isn't a directory
    {
        cout << "Travel error: " << travelPath << " is not a directory" << endl;
        return;
    }

    if(isOutputPathSupplied == false)
    {
        this->outputPath = travelPath;
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
    Errors errors;

    const std::filesystem::path pathOfTravel = travelPath;
    const string travelName = pathOfTravel.stem().string();
    const std::filesystem::path pathOfOutputFilesForAlgorithmAndTravel = outputPath + "/output_of_" + algorithmName + "_" + travelName; //TODO: replace algorithm name
    string outputPathOfErrorsFile = pathOfOutputFilesForAlgorithmAndTravel.string() + "/errors_of_" + algorithmName + "_" + travelName + ".csv";
    std::filesystem::create_directory(pathOfOutputFilesForAlgorithmAndTravel);

    cout << "Reading route" << endl;
    errors.addError(IO::readShipRoute(travelPath + "/route.txt", route));
    ports = route.getPorts();
    for(string port : ports)
    {
        totalNumbersOfVisitingPort[port]++;
    }
    algorithm->readShipRoute(travelPath + "/route.txt");

    cout << "Reading ship plan" << endl;
    errors.addError(IO::readShipPlan(travelPath + "/ship_plan.txt", ship.getShipPlan()));
    algorithm->readShipPlan(travelPath + "/ship_plan.txt");

    for (size_t i = 0; i < ports.size(); i++)
    {
        vector<Container> containers;
        vector<Container> badContainers;
        std::cout << "----------------" <<"Ship Arrived to port " << ports[i] << "----------------" << std::endl;
        const std::filesystem::path pathOfContainersAwaitingAtPortFile = travelPath + "/" + ports[i] + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".cargo_data.txt"; //TODO: remove .txt in linux

        if(std::filesystem::exists(pathOfContainersAwaitingAtPortFile) == 0)
        {
            std::cout << "No containers awaiting at port input file for " << ports[i] << " for visiting number " << indexOfVisitAtPort[ports[i]] + 1 << std::endl;
            algorithm->getInstructionsForCargo(travelPath + "/" + "empty_containers_awaiting_at_port_file.txt", pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + algorithmName + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".txt");
        }
        else
        {
            IO::readContainerAwaitingAtPortFile(pathOfContainersAwaitingAtPortFile.string(),ship,containers,badContainers);
            algorithm->getInstructionsForCargo(pathOfContainersAwaitingAtPortFile.string(), pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + algorithmName + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".txt");
        }

        craneManagementAnswer = craneManagement.readAndExecuteInstructions(ship, pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + algorithmName + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".txt");
        totalOperations += craneManagementAnswer.numOfOperations;
        checkForErrorsAfterPort(ship, ports[i], craneManagementAnswer, route, pathOfOutputFilesForAlgorithmAndTravel.string(),containers, badContainers);
        indexOfVisitAtPort[ports[i]]++;
        std::cout << "----------------" <<"Ship left port " << ports[i] << "----------------" << std::endl;
        route.incrementCurrentPort();

    }

    algorithmsResults[algorithmName].addTravelResult(travelName, totalOperations);

    IO::writeErrorsOfTravelAndAlgorithm(errors, outputPathOfErrorsFile);

    std::cout <<"===========================================================" << std::endl;
    std::cout <<"finished simulate " << algorithmName << " on " << travelPath << std::endl;
    std::cout <<"===========================================================" << std::endl;

}

void Simulation::checkForErrorsAfterPort(Ship &ship, const string &port, CraneManagement::CraneManagementAnswer &answer,
                                         Route &route, const string &pathOfOutputFilesForAlgorithmAndTravel,
                                         vector<Container> &containers, vector<Container> &badContainers) {
    Errors errors;

    // --------------- check all containers supposed to be unloaded were unloaded ---------------
    if (ship.portToContainers[port].size() > 0) {
        string error = "Not all of the containers with of this port destination were unloaded";
        cout << error << std::endl;
        IO::writeToFile(pathOfOutputFilesForAlgorithmAndTravel, error + ",");
    }

    vector<string> rejected = answer.changedContainers[Action::REJECT];
    int size = rejected.size();
    for (auto &container : badContainers) {
        bool found = false;
        for (int i = 0  ; i < size ; i++) {
            if (!rejected[i].compare(container.getId())) {
                found = true;
                rejected.erase(rejected.begin()+i);
                break;
            }
        }
        if (!found){
            errors.addError(Error::BAD_CONTAINER_WASNT_REJECTED);
        }
    }

    // ---------------------- check all good waiting containers were reviewed ----------------------
    for (auto &container : containers) {
        bool inRejected = false, inLoaded = false;
        bool destIsCurrentPort = ship.containerIdToDestination(container.getId()) == route.getCurrentPortName();
        bool destIsntInNextStops = !route.portInNextStops(ship.containerIdToDestination(container.getId()));
        bool destIsFar = !checkIfCloserContainerWasLoaded(ship, answer, route, container);

        for (string &id : answer.changedContainers[Action::REJECT]) {
            if (!id.compare(container.getId())) {
                if (!destIsCurrentPort && !destIsntInNextStops && !destIsFar){
                    errors.addError(Error::REJECTED_NOT_FAR_CONTAINERS);
                }
                inRejected = true;
                break;
            }
        }
        for (string &id : answer.changedContainers[Action::LOAD]) {
            if (!id.compare(container.getId())) {
                if (destIsCurrentPort){
                    errors.addError(Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT);
                }
                if (destIsntInNextStops){
                    errors.addError(Error::CONTAINER_DESTINATION_ISNT_IN_NEXT_STOPS);
                }
                inLoaded = true;
                break;
            }
        }
        if (!inRejected && !inLoaded) {
            errors.addError(Error::CONTAINER_WASNT_REVIEWED);
        }
    }

    // --------------- check if ship is empty in end of travel ---------------
    if (route.inLastStop()) {
        int amountOfContainers = ship.getAmountOfContainers() > 0;
        if (amountOfContainers > 0) {
            errors.addError(Error::SHIP_ISNT_EMPTY_IN_END_OF_TRAVEL);
        }
    }
}

bool Simulation::checkIfCloserContainerWasLoaded(Ship &ship,  CraneManagement::CraneManagementAnswer &answer,
                                                 Route &route, Container &container)  {
    bool closerContainerExists = false;
    for (auto &loaded : answer.changedContainers[Action::LOAD]) {
        if (route.nextStopForPort(ship.containerIdToDestination(container.getId())) <
            route.nextStopForPort(ship.containerIdToDestination(loaded))) {
            closerContainerExists = true;
        }
    }
    return closerContainerExists;
}

int Simulation::checkTravelsPath(const string &travelsPathToCheck) {
    if (travelsPathToCheck == "")
    {
        cout << "Fatal error: missing travel_path argument!" << endl;
        return 1;
    }

    if(std::filesystem::exists(travelsPathToCheck) == 0)
    {
        cout << "Fatal error: travel_path argument isn't valid!" << endl;
        return 1;
    }
    return 0;
}





