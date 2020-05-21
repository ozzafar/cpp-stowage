//
// Created by Yarden on 03/05/2020.
//


#include "Simulation.h"
#include "CheckAlgorithmError.h"
#include <vector>

#ifndef RUNNING_ON_NOVA
#include "../algorithm/_206039984_a.h"
#include "../algorithm/_206039984_b.h"
#include "../algorithm/_206039984_c.h"
#endif


Simulation::Simulation(const string &travelsPath, const string &algorithmPath, const string &outputPath): travelsPath(travelsPath), algorithmPath(algorithmPath), outputPath(outputPath)
{
#ifndef RUNNING_ON_NOVA
    Registrar::getInstance().factoryVec.emplace_back([](){return std::make_unique<_206039984_a>();});
    Registrar::getInstance().factoryVec.emplace_back([](){return std::make_unique<_206039984_b>();});
    Registrar::getInstance().factoryVec.emplace_back([](){return std::make_unique<_206039984_c>();});
    Registrar::getInstance().addName("_206039984_a");
    Registrar::getInstance().addName("_206039984_b");
    Registrar::getInstance().addName("_206039984_c");
#endif


    if(outputPath!="")
    {
        this->outputPath = outputPath;
        isOutputPathSupplied = true;
    }
    else
    {
        string h = std::filesystem::current_path().string();
        this->outputPath = std::filesystem::current_path().string();
    }
    IO::clearPreviousOutput(this->outputPath);

    if(algorithmPath == "")
    {
        this->algorithmPath = std::filesystem::current_path().string();
    }

    int size = 0;
    for(auto& so: std::filesystem::directory_iterator(algorithmPath))
    {
        string path = so.path().string();
        if(path.substr(path.find_last_of(".") + 1) == "so")
        {
            Registrar::getInstance().loadSO(path);
            if (Registrar::getInstance().factoryVec.size() - size != 1){
               // CheckAlgorithmError::FAILED_TO_LOAD_ALGORITHM // TODO handle this error
            }
            size = Registrar::getInstance().factoryVec.size();
        }
    }
    std::cout << "final factory size: "  << Registrar::getInstance().factoryVec.size() << std::endl;

    for(unsigned long long i = 0 ; i < Registrar::getInstance().factoryVec.size(); i++)
    {
        algorithmsResults[Registrar::getInstance().names[i]] = AlgorithmResults(Registrar::getInstance().names[i]);
    }
}

int Simulation::simulateAllTravelsWithAllAlgorithms()
{
    if (checkTravelsPath(travelsPath)!=0)
    {
        std::cout << "Error: invalid travels path. try again." << std::endl;
        return 1;
    }

    string outputPathWithoutLastDirectory = outputPath.substr(0, outputPath.find_last_of("/"));

    if(!std::filesystem::exists(outputPathWithoutLastDirectory))
    {
        IO::writeToFile(std::filesystem::current_path().string() + "/simulation_errors.csv", "INVALID OUTPUT PATH");
        std::cout << "Error: cannot create/find output path. Please Enter Correct path." << std::endl;
        return 1;
    }

    if(!std::filesystem::exists(outputPath))
    {
        std::filesystem::create_directory(outputPath);
    }

    std::filesystem::create_directory(outputPath + "/errors");

    if(Registrar::getInstance().factoryVec.size() == 0)
    {
        std::cout << "Error: failed to load algorithms. Try again or enter different path" << std::endl;
        return 1;
    }

    for(auto& travel: std::filesystem::directory_iterator(travelsPath))
    {
        string pathOfCurrentTravel = travel.path().string();
        travelNames.push_back(travel.path().stem().string());
        simulateOneTravelWithAllAlgorithms(pathOfCurrentTravel);
    }

    string resultOutputPath = outputPath + "/simulation.results";
    IO::writeResultsOfSimulation(resultOutputPath, travelNames, algorithmsResults);

    return 0;
}

void Simulation::simulateOneTravelWithAllAlgorithms(const string &travelPath) {

    std::cout << "Started simulate " << travelPath << std::endl;
    Errors simErrors;

    const std::filesystem::path path = travelPath;
    if(is_directory(path) == 0) //travelPath isn't a directory
    {
        std::cout << "Travel error: " << travelPath << " is not a directory" << std::endl;
        travelNames.pop_back();
        return;
    }

    std::ofstream file { travelPath + "/empty_containers_awaiting_at_port_file" };
    string travelName = path.stem().string();

    Registrar &registrar = Registrar::getInstance();
    for(size_t i = 0 ; i < registrar.factoryVec.size() ; i++ )
    {
        if(simErrors.hasTravelError() == 0)
        {
            std::unique_ptr<AbstractAlgorithm> algorithm = registrar.factoryVec[i]();
            simErrors.addErrors(simulateOneTravelWithOneAlgorithm(travelPath, std::move(algorithm), registrar.names[i]));
        }
        else
        {
            std::cout << "travel error" << std::endl;
            //TODO: write to error file that travel error occurred
        }
        if (simErrors.hasTravelError() == 1)
        {
            travelNames.erase(std::remove(travelNames.begin(), travelNames.end(), travelName), travelNames.end());
        }
    }

    std::cout << "Finished simulate " << travelPath << std::endl;
}

Errors Simulation::simulateOneTravelWithOneAlgorithm(const string &travelPath, std::unique_ptr<AbstractAlgorithm> algorithm, const string &algorithmName) {

    std::cout <<"===========================================================" << std::endl;
    std::cout <<"started simulate " << algorithmName << " on " << travelPath << std::endl;
    std::cout <<"===========================================================" << std::endl;

    Errors simErrors,algErrors;
    int totalOperations = 0;
    Route route;
    vector<string> ports;
    Ship ship;
    CraneManagement craneManagement;
    CraneManagement::CraneManagementAnswer craneManagementAnswer;
    WeightBalanceCalculator weightBalanceCalculator;
    std::map<string,int> indexOfVisitAtPort;
    std::map<string,int> totalNumbersOfVisitingPort;
    const std::filesystem::path pathOfTravel = travelPath;
    const string travelName = pathOfTravel.stem().string();
    const std::filesystem::path pathOfOutputFilesForAlgorithmAndTravel = outputPath + "/" + algorithmName + "_" + travelName + "_crane_instructions";
    string outputPathOfErrorsFile = outputPath + "/errors/errors_of_" + algorithmName + "_" + travelName + ".csv";
    std::filesystem::create_directory(pathOfOutputFilesForAlgorithmAndTravel);

    algErrors.addErrors(algorithm->setWeightBalanceCalculator(weightBalanceCalculator));

    if(simErrors.hasTravelError())
    {
        IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors,outputPathOfErrorsFile);
        return simErrors;
    }

    std::cout << "Reading route" << std::endl;
    simErrors.addErrors(IO::readShipRoute(travelPath, route));
    if(simErrors.hasTravelError())
    {
        IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
        return simErrors;
    }
    ports = route.getPorts();
    for(string port : ports)
    {
        totalNumbersOfVisitingPort[port]++;
    }
    algErrors.addErrors(algorithm->readShipRoute(travelPath));
    if(algErrors.hasTravelError())
    {
        //TODO: change this block. simulator read route without problems but algorithm says that there is
        // a problem. meaning algorithm fault. result should be -1 and proper error should be updated for return
        IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
        return simErrors;
    }

    std::cout << "Reading ship plan" << std::endl;
    simErrors.addErrors(IO::readShipPlan(travelPath, ship.getShipPlan()));
    if(simErrors.hasTravelError())
    {
        std::cout << "simulation travel error" << std::endl;
        IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
        return simErrors;
    }
    algErrors.addErrors(algorithm->readShipPlan(travelPath));
    if(algErrors.hasTravelError())
    {
        std::cout << "algorithm travel error" << std::endl;
        //TODO: change this block. simulator read ship plan without problems but algorithm says that there is
        // a problem. meaning algorithm fault. result should be -1 and proper error should be updated for return
        IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
        return simErrors;
    }

    for (size_t i = 0; i < ports.size(); i++)
    {
        vector<Container> containers;
        vector<Container> badContainers;
        std::cout << "----------------" <<"Ship Arrived to port " << ports[i] << "----------------" << std::endl;
        const std::filesystem::path pathOfContainersAwaitingAtPortFile = travelPath + "/" + ports[i] + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".cargo_data";

        if(std::filesystem::exists(pathOfContainersAwaitingAtPortFile) == 0)
        {
            std::cout << "No containers awaiting at port input file for " << ports[i] << " for visiting number " << indexOfVisitAtPort[ports[i]] + 1 << std::endl;
            algErrors.addErrors(algorithm->getInstructionsForCargo(travelPath + "/" + "empty_containers_awaiting_at_port_file", pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".crane_instructions"));
            if(algErrors.hasTravelError())
            {
                IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
                return simErrors;
            }
        }
        else
        {
            simErrors.addErrors(IO::readContainerAwaitingAtPortFile(pathOfContainersAwaitingAtPortFile.string(),ship,containers,badContainers));
            if (route.inLastStop() && (!containers.empty() || !badContainers.empty()))
            {
                // CheckAlgorithmError::SHIP_HAS_CONTAINERS_AT_THE_END_OF_THE_ROUTE); TODO handle it
                return simErrors;
            }
            if(simErrors.hasTravelError())
            {
                IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
                return simErrors;
            }
            algErrors.addErrors(algorithm->getInstructionsForCargo(pathOfContainersAwaitingAtPortFile.string(), pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".crane_instructions"));
            if(algErrors.hasTravelError())
            {
                IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
                return simErrors;
            }
        }

        string j = pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".crane_instructions";
        craneManagementAnswer = craneManagement.readAndExecuteInstructions(ship, pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".crane_instructions");
        simErrors.addErrors(craneManagementAnswer.errors);
        if(simErrors.hasError(Error::ALGORITHM_INVALID_COMMAND))
        {
            IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
            return simErrors;
        }
        totalOperations += craneManagementAnswer.numOfOperations;
        simErrors.addErrors(checkForErrorsAfterPort(ship, ports[i], craneManagementAnswer, route,containers, badContainers));
        //TODO: add here check for algorithm errors, and stop the iteration if needed
        indexOfVisitAtPort[ports[i]]++;
        std::cout << "----------------" <<"Ship left port " << ports[i] << "----------------" << std::endl;
        route.incrementCurrentPort();

    }

    algorithmsResults[algorithmName].addTravelResult(travelName, totalOperations);
    IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);

    std::cout <<"===========================================================" << std::endl;
    std::cout <<"finished simulate " << algorithmName << " on " << travelPath << std::endl;
    std::cout <<"===========================================================" << std::endl;

    return simErrors;

}

int Simulation::checkForErrorsAfterPort(Ship& ship, const string &port, CraneManagement::CraneManagementAnswer& answer,Route& route, vector<Container>& containers,vector<Container> &badContainers) {

    Errors errors;

    // --------------- check all containers supposed to be unloaded were unloaded ---------------
    if (ship.portToContainers[port].size() > 0) {
        std::cout << "Not all of the containers with of this port destination were unloaded" << std::endl;
        // CheckAlgorithmError::CONTAINERS_SHOULD_BE_UNLOADED_SKIPPED_BY_THE_ALGORITHM; TODO report it to errors file and return -1
    }

    // --------------- check all bad containers were rejected ---------------
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
            std::cout << "algorithm didn't reject bad container" << std::endl;
            //CheckAlgorithmError::BAD_CONTAINER_WASNT_REJECTED; TODO report it to errors file and return -1
        }
    }

    // ---------------------- check all good waiting containers were reviewed ----------------------
    for (auto &container : containers) {
        bool inRejected = false, inLoaded = false;
        bool destIsCurrentPort = ship.containerIdToDestination(container.getId()) == route.getCurrentPortName();
        bool destIsntInNextStops = !route.portInNextStops(ship.containerIdToDestination(container.getId()));
        bool destIsFar = checkIfAllLoadedContainersAreCloser(ship, answer, route, container);

        for (string &id : answer.changedContainers[Action::REJECT]) {
            // container isn't bad because all bad were removed
            if (!id.compare(container.getId())) {
                if (!destIsCurrentPort && !destIsntInNextStops) {
                    // there is no reason to reject related the container itself,
                    // it must be because of lack of place - container must be farther then the loaded containers
                    if (!destIsFar) {
                        // CheckAlgorithmError::REJECTED_NOT_FAR_CONTAINERS TODO report it to errors file and return -1
                        inRejected = true;
                        break;
                    }
                }
            }
        }

        for (string &id : answer.changedContainers[Action::LOAD]) {
            if (!id.compare(container.getId())) {
                if (destIsCurrentPort){
                    // CheckAlgorithmError::LOADED_PORT_DESTINATION_IS_CURRENT_PORT TODO report it to errors file and return -1
                }
                if (destIsntInNextStops){
                    // CheckAlgorithmError::CONTAINER_DESTINATION_ISNT_IN_NEXT_STOPS TODO report it to errors file and return -1
                }
                inLoaded = true;
                break;
            }
        }
        if (!inRejected && !inLoaded) {
            std::cout << "algorithm didn't review some waiting-in-port containers" << std::endl;
            // CheckAlgorithmError::CONTAINER_WASNT_REVIEWED TODO report it to errors file and return -1
        }
    }

    // --------------- check if ship is empty in end of travel ---------------
    if (route.inLastStop()) {
        int amountOfContainers = ship.getAmountOfContainers() > 0;
        if (amountOfContainers > 0) {
            //CheckAlgorithmError::SHIP_ISNT_EMPTY_IN_END_OF_TRAVEL TODO report it to errors file and return -1
        }
    }

    return errors.getErrorsCode();
}

bool Simulation::checkIfAllLoadedContainersAreCloser(Ship &ship, CraneManagement::CraneManagementAnswer &answer,
                                                     Route &route, Container &container)  {
    bool allCloser = true;
    for (auto &loaded : answer.changedContainers[Action::LOAD]) {
        if (route.nextStopForPort(ship.containerIdToDestination(container.getId())) <
            route.nextStopForPort(ship.containerIdToDestination(loaded))) {
            allCloser = false;
            break;
        }
    }
    return allCloser;
}

int Simulation::checkTravelsPath(const string &travelsPathToCheck) {
    if (travelsPathToCheck == "")
    {
        std::cout << "Fatal error: missing travel_path argument!" << std::endl;
        return 1;
    }

    if(std::filesystem::exists(travelsPathToCheck) == 0)
    {
        std::cout << "Fatal error: travel_path argument isn't valid!" << std::endl;
        return 1;
    }
    return 0;
}





