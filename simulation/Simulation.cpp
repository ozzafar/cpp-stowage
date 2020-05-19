//
// Created by Yarden on 03/05/2020.
//


#include "Simulation.h"


Simulation::Simulation(const string &travelsPath, const string &algorithmPath, const string &outputPath): travelsPath(travelsPath), algorithmPath(algorithmPath), outputPath(outputPath)
{
#ifndef RUNNING_ON_NOVA
    Registrar::getInstance().factoryVec.emplace_back([](){return std::make_unique<AbstractAlgorithm>();});
    Registrar::getInstance().addName("_206039984_a");
#endif
    //TODO: clean previous output/error/result files(?)

    if(outputPath!="")
    {
        this->outputPath = outputPath + "/output";
        isOutputPathSupplied = true;
    }
    else
    {
        this->outputPath = std::filesystem::current_path().string() + "/output";
    }

    if(algorithmPath == "")
    {
        this->algorithmPath = std::filesystem::current_path().string();
    }


    for(auto& so: std::filesystem::directory_iterator(algorithmPath))
    {

        string path = so.path().string();
        if(path.substr(path.find_last_of(".") + 1) == "so")
        {
            std::cout << path << std::endl;
            //Registrar::getInstance().addName(so.path().stem().string());
            Registrar::getInstance().loadSO(path);
            std::cout << "factory size: "  << Registrar::getInstance().factoryVec.size() << std::endl;
        }
    }

    for(unsigned long long i = 0 ; i < Registrar::getInstance().factoryVec.size(); i++)
    {
        algorithmsResults[Registrar::getInstance().names[i]] = AlgorithmResults(Registrar::getInstance().names[i]);
    }
}

int Simulation::simulateAllTravelsWithAllAlgorithms()
{
    if (checkTravelsPath(travelsPath)!=0)
    {
        return 1;
    }

    bool directoryCreated = std::filesystem::create_directory(outputPath);

    if(!directoryCreated && !std::filesystem::exists(outputPath))
    {
        std::cout << "Error: cannot create/find output path. Please Enter Correct path." << std::endl;
        return 1;
    }

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

    //TODO: sort algorithmResults
    string resultOutputPath = outputPath + "/simulation.results.csv";
    IO::writeResultsOfsimulation(resultOutputPath, travelNames, algorithmsResults);

    return 0;
}

void Simulation::simulateOneTravelWithAllAlgorithms(const string &travelPath) {

    std::cout << "Started simulate " << travelPath << std::endl;
    Errors errors;

    const std::filesystem::path path = travelPath;
    if(is_directory(path) == 0) //travelPath isn't a directory
    {
        std::cout << "Travel error: " << travelPath << " is not a directory" << std::endl;
        travelNames.pop_back();
        return;
    }

    std::ofstream file { travelPath + "/empty_containers_awaiting_at_port_file.txt" };


    Registrar &registrar = Registrar::getInstance();
    for(size_t i = 0 ; i < registrar.factoryVec.size() ; i++ )
    {
        if(errors.hasTravelError() == 0)
        {
            std::unique_ptr<AbstractAlgorithm> algorithm = registrar.factoryVec[i]();
            errors = simulateOneTravelWithOneAlgorithm(travelPath, std::move(algorithm), registrar.names[i]);
        }
        else
        {
            std::cout << "travel error" << std::endl;
            //TODO: write to error file that travel error occurred
            algorithmsResults[registrar.names[i]].addTravelResult(path.stem().string(), -1);
        }
    }

    std::cout << "Finished simulate " << travelPath << std::endl;
}

Errors Simulation::simulateOneTravelWithOneAlgorithm(const string &travelPath, std::unique_ptr<AbstractAlgorithm> algorithm, const string &algorithmName) {

    std::cout <<"===========================================================" << std::endl;
    std::cout <<"started simulate " << algorithmName << " on " << travelPath << std::endl;
    std::cout <<"===========================================================" << std::endl;

    Errors errors;
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
    const std::filesystem::path pathOfOutputFilesForAlgorithmAndTravel = outputPath + "/output_of_" + algorithmName + "_" + travelName;
    string outputPathOfErrorsFile = pathOfOutputFilesForAlgorithmAndTravel.string() + "/errors_of_" + algorithmName + "_" + travelName + ".csv";
    std::filesystem::create_directory(pathOfOutputFilesForAlgorithmAndTravel);

    errors.addError(algorithm->setWeightBalanceCalculator(weightBalanceCalculator));

    if(errors.hasTravelError())
    {
        IO::writeErrorsOfTravelAndAlgorithm(errors, outputPathOfErrorsFile);
        algorithmsResults[algorithmName].addTravelResult(travelName, -1);
        return errors;
    }

    std::cout << "Reading route" << std::endl;
    errors.addError(IO::readShipRoute(travelPath + "/route.txt", route));
    if(errors.hasTravelError())
    {
        IO::writeErrorsOfTravelAndAlgorithm(errors, outputPathOfErrorsFile);
        algorithmsResults[algorithmName].addTravelResult(travelName, -1);
        return errors;
    }
    ports = route.getPorts();
    for(string port : ports)
    {
        totalNumbersOfVisitingPort[port]++;
    }
    errors.addError(algorithm->readShipRoute(travelPath + "/route.txt"));
    if(errors.hasTravelError())
    {
        IO::writeErrorsOfTravelAndAlgorithm(errors, outputPathOfErrorsFile);
        algorithmsResults[algorithmName].addTravelResult(travelName, -1);
        return errors;
    }

    std::cout << "Reading ship plan" << std::endl;
    errors.addError(IO::readShipPlan(travelPath + "/ship_plan.txt", ship.getShipPlan()));
    if(errors.hasTravelError())
    {
        IO::writeErrorsOfTravelAndAlgorithm(errors, outputPathOfErrorsFile);
        algorithmsResults[algorithmName].addTravelResult(travelName, -1);
        return errors;
    }
    errors.addError(algorithm->readShipPlan(travelPath + "/ship_plan.txt"));
    if(errors.hasTravelError())
    {
        IO::writeErrorsOfTravelAndAlgorithm(errors, outputPathOfErrorsFile);
        algorithmsResults[algorithmName].addTravelResult(travelName, -1);
        return errors;
    }

    for (size_t i = 0; i < ports.size(); i++)
    {
        vector<Container> containers;
        vector<Container> badContainers;
        std::cout << "----------------" <<"Ship Arrived to port " << ports[i] << "----------------" << std::endl;
        const std::filesystem::path pathOfContainersAwaitingAtPortFile = travelPath + "/" + ports[i] + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".cargo_data.txt";

        if(std::filesystem::exists(pathOfContainersAwaitingAtPortFile) == 0)
        {
            std::cout << "No containers awaiting at port input file for " << ports[i] << " for visiting number " << indexOfVisitAtPort[ports[i]] + 1 << std::endl;
            errors.addError(algorithm->getInstructionsForCargo(travelPath + "/" + "empty_containers_awaiting_at_port_file.txt", pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + algorithmName + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".txt"));
            if(errors.hasTravelError())
            {
                IO::writeErrorsOfTravelAndAlgorithm(errors, outputPathOfErrorsFile);
                algorithmsResults[algorithmName].addTravelResult(travelName, -1);
                return errors;
            }
        }
        else
        {
            errors.addError(IO::readContainerAwaitingAtPortFile(pathOfContainersAwaitingAtPortFile.string(),ship,containers,badContainers));
            if(errors.hasTravelError())
            {
                IO::writeErrorsOfTravelAndAlgorithm(errors, outputPathOfErrorsFile);
                algorithmsResults[algorithmName].addTravelResult(travelName, -1);
                return errors;
            }
            errors.addError(algorithm->getInstructionsForCargo(pathOfContainersAwaitingAtPortFile.string(), pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + algorithmName + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".txt"));
            if(errors.hasTravelError())
            {
                IO::writeErrorsOfTravelAndAlgorithm(errors, outputPathOfErrorsFile);
                algorithmsResults[algorithmName].addTravelResult(travelName, -1);
                return errors;
            }
        }

        craneManagementAnswer = craneManagement.readAndExecuteInstructions(ship, pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + algorithmName + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".txt");
        errors.addError(craneManagementAnswer.errors);
        if(errors.hasError(Error::ALGORITHM_INVALID_COMMAND))
        {
            IO::writeErrorsOfTravelAndAlgorithm(errors, outputPathOfErrorsFile);
            algorithmsResults[algorithmName].addTravelResult(travelName, -1);
            return errors;
        }
        totalOperations += craneManagementAnswer.numOfOperations;
        checkForErrorsAfterPort(ship, ports[i], craneManagementAnswer, route,containers, badContainers);
        indexOfVisitAtPort[ports[i]]++;
        std::cout << "----------------" <<"Ship left port " << ports[i] << "----------------" << std::endl;
        route.incrementCurrentPort();

    }

    algorithmsResults[algorithmName].addTravelResult(travelName, totalOperations);
    IO::writeErrorsOfTravelAndAlgorithm(errors, outputPathOfErrorsFile);

    std::cout <<"===========================================================" << std::endl;
    std::cout <<"finished simulate " << algorithmName << " on " << travelPath << std::endl;
    std::cout <<"===========================================================" << std::endl;

    return errors;

}

int Simulation::checkForErrorsAfterPort(Ship& ship, const string &port, CraneManagement::CraneManagementAnswer& answer,Route& route, vector<Container>& containers,vector<Container> &badContainers) {

    Errors errors;

    // --------------- check all containers supposed to be unloaded were unloaded ---------------
    if (ship.portToContainers[port].size() > 0) {
        errors.addError(Error::CONTAINERS_SHOULD_BE_UNLOADED_SKIPPED_BY_THE_ALGORITHM);
        std::cout << "Not all of the containers with of this port destination were unloaded" << std::endl;
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
            errors.addError(Error::BAD_CONTAINER_WASNT_REJECTED);
        }
    }

    // ---------------------- check all good waiting containers were reviewed ----------------------
    for (auto &container : containers) {
        bool inRejected = false, inLoaded = false;
        bool destIsCurrentPort = ship.containerIdToDestination(container.getId()) == route.getCurrentPortName();
        bool destIsntInNextStops = !route.portInNextStops(ship.containerIdToDestination(container.getId()));
        bool destIsFar = checkIfAllLoadedContainersAreCloser(ship, answer, route, container);

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





