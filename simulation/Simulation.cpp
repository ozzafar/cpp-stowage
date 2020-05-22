//
// Created by Yarden on 03/05/2020.
//


#include "Simulation.h"
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
                IO::writeToFile(outputPath + "/general_errors.csv", so.path().stem().string() + " couldn't be loaded,");
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
        }
        if (simErrors.hasTravelError() == 1)
        {
            IO::writeToFile(outputPath + "/general_errors.csv", travelName + " has travel error,");
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

    std::cout << "Reading route" << std::endl;
    string routePath = IO::firstFileWithExtensionInDirectory(travelPath, "route");
    simErrors.addErrors(IO::readShipRoute(routePath, route));
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


    algErrors.addErrors(algorithm->readShipRoute(routePath));
    if(algErrors.hasTravelError())
    {
        algorithmsResults[algorithmName].addTravelResult(travelName, -1);
        IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
        IO::writeToFile(outputPathOfErrorsFile, Errors::errorToString(Error::ALGORITHM_FALSE_TRAVEL_ERROR));
        return simErrors;
    }

    std::cout << "Reading ship plan" << std::endl;
    string shipPlanPath = IO::firstFileWithExtensionInDirectory(travelPath, "ship_plan");
    simErrors.addErrors(IO::readShipPlan(shipPlanPath, ship.getShipPlan()));
    if(simErrors.hasTravelError())
    {
        std::cout << "simulation travel error" << std::endl;
        IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
        return simErrors;
    }
    algErrors.addErrors(algorithm->readShipPlan(shipPlanPath));
    if(algErrors.hasTravelError())
    {
        algorithmsResults[algorithmName].addTravelResult(travelName, -1);
        IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
        IO::writeToFile(outputPathOfErrorsFile, Errors::errorToString(Error::ALGORITHM_FALSE_TRAVEL_ERROR));
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
        }
        else
        {
            simErrors.addErrors(IO::readContainerAwaitingAtPortFile(pathOfContainersAwaitingAtPortFile.string(),ship,containers,badContainers));
            if (route.inLastStop() && (!containers.empty() || !badContainers.empty()))
            {
                simErrors.addError(Error::LAST_PORT_HAS_CONTAINERS_WARNING);
            }
            algErrors.addErrors(algorithm->getInstructionsForCargo(pathOfContainersAwaitingAtPortFile.string(), pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".crane_instructions"));
        }

        if(algErrors.hasTravelError())
        {
            algorithmsResults[algorithmName].addTravelResult(travelName, -1);
            IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
            IO::writeToFile(outputPathOfErrorsFile, Errors::errorToString(Error::ALGORITHM_FALSE_TRAVEL_ERROR));
            return simErrors;
        }

        craneManagementAnswer = craneManagement.readAndExecuteInstructions(ship, pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".crane_instructions");
        simErrors.addErrors(craneManagementAnswer.errors);
        if(craneManagementAnswer.errors != (int) Error::SUCCESS)
        {
            algorithmsResults[algorithmName].addTravelResult(travelName, -1);
            IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
            return simErrors;
        }
        totalOperations += craneManagementAnswer.numOfOperations;
        if(travelName == "travel3")
        {
            std::cout << "travel 3" << std::endl;
        }
        int checkForErrorsAfterPortResult = checkForErrorsAfterPort(ship, ports[i], craneManagementAnswer, route,containers, badContainers);
        simErrors.addErrors(checkForErrorsAfterPortResult);
        if(checkForErrorsAfterPortResult != (int) Error::SUCCESS)
        {
            algorithmsResults[algorithmName].addTravelResult(travelName, -1);
            IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
            return simErrors;
        }

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
        errors.addError(Error::ALGORITHM_IGNORED_CONTAINER_SHOULD_BE_UNLOADED);
        return errors.getErrorsCode();
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
            std::cout << "Error: algorithm didn't reject bad container" << std::endl;
            errors.addError(Error::ALGORITHM_BAD_CONTAINER_WASNT_REJECT);
            return errors.getErrorsCode();
        }
    }

    // ---------------------- check all good waiting containers were reviewed ----------------------
    for (auto &container : containers) {
        bool inRejected = false, inLoaded = false;
        bool destIsCurrentPort = ship.containerIdToDestination(container.getId()) == route.getCurrentPortName();
        bool destIsntInNextStops = !route.portInNextStops(ship.containerIdToDestination(container.getId()));
        bool destIsFar = checkIfAllLoadedContainersAreCloser(ship, answer, route, container);

        for (string &id : rejected) {
            // container isn't bad because all bad were removed
            if (!id.compare(container.getId())) {
                if (!destIsCurrentPort && !destIsntInNextStops) {
                    // there is no reason to reject related the container itself,
                    // it must be because of lack of place - container must be farther then the loaded containers
                    if (!destIsFar) {
                        errors.addError(Error::ALGORITHM_INCORRECTLY_REJECTED_CONTAINER);
                        std::cout << "Error: algorithm rejected not far container" << std::endl;
                        //TODO: specify reason
                        return errors.getErrorsCode();
                    }
                }
                inRejected = true;
                break;
            }
        }

        for (string &id : answer.changedContainers[Action::LOAD]) {
            if (!id.compare(container.getId())) {
                if (destIsCurrentPort){
                    errors.addError(Error::ALGORITHM_NOT_ALLOWED_INSTRUCTION);
                    std::cout << "Error: algorithm loaded container with destination as current port" << std::endl;
                    return errors.getErrorsCode();
                }
                if (destIsntInNextStops){
                    errors.addError(Error::ALGORITHM_NOT_ALLOWED_INSTRUCTION);
                    std::cout << "Error: algorithm loaded a container not in destination" << std::endl;
                    return errors.getErrorsCode();
                }
                inLoaded = true;
                break;
            }
        }
        if (!inRejected && !inLoaded) {
            std::cout << "Error: algorithm didn't review some waiting-in-port containers" << std::endl;
            errors.addError(Error::ALGORITHM_IGNORED_CONTAINER_SHOULD_BE_LOADED);
            return errors.getErrorsCode();
        }
    }

    // --------------- check if ship is empty in end of travel ---------------
    if (route.inLastStop()) {
        int amountOfContainers = ship.getAmountOfContainers() > 0;
        if (amountOfContainers > 0) {
            errors.addError(Error::SHIP_ISNT_EMPTY_IN_END_OF_TRAVEL);
            std::cout << "Error: algorithm finished with nonempty ship" << std::endl;
            return errors.getErrorsCode();
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





