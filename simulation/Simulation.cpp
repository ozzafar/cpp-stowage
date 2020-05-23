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
        this->outputPath = std::filesystem::current_path().string();
    }
    IO::clearPreviousOutput(this->outputPath);

    if(algorithmPath == "")
    {
        this->algorithmPath = std::filesystem::current_path().string();
    }

    int size = 0;

    //load so algorithms objects
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
    std::cout << Registrar::getInstance().factoryVec.size() << " algorithms registered" << std::endl;

    for(unsigned long long i = 0 ; i < Registrar::getInstance().factoryVec.size(); i++)
    {
        algorithmsResults[Registrar::getInstance().names[i]] = AlgorithmResults(Registrar::getInstance().names[i]);
    }
}

int Simulation::simulateAllTravelsWithAllAlgorithms()
{
    if (checkTravelsPath(travelsPath) != 0)
    {
        std::cout << "Error: invalid travels path. try again." << std::endl;
        return 1;
    }

    string outputPathWithoutLastDirectory = outputPath.substr(0, outputPath.find_last_of("/"));

    //Check if path to desired outpath is correct
    if(!std::filesystem::exists(outputPathWithoutLastDirectory))
    {
        IO::writeToFile(std::filesystem::current_path().string() + "/simulation_errors.csv", "INVALID OUTPUT PATH");
        std::cout << "Error: cannot create/find output path. Please Enter Correct path." << std::endl;
        return 1;
    }

    //Create the output directory if needed
    if(!std::filesystem::exists(outputPath))
    {
        std::filesystem::create_directory(outputPath);
    }

    std::filesystem::create_directory(outputPath + "/errors");

    //if no algorithm was loaded
    if(Registrar::getInstance().factoryVec.size() == 0)
    {
        std::cout << "Error: failed to load algorithms. Try again or enter different path" << std::endl;
        IO::writeToFile(outputPath + "/general_errors.csv", "failed to load algorithms,");
        return 1;
    }

    //iterate over folders in "travels" directory
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

void Simulation::simulateOneTravelWithAllAlgorithms(const string &travelPath)
{
    std::cout << "Started simulate " << travelPath << std::endl;
    Errors simErrors;

    const std::filesystem::path path = travelPath;

    //travelPath isn't a directory
    if(is_directory(path) == 0)
    {
        std::cout << "\tTravel error: " << travelPath << " is not a directory" << std::endl;
        travelNames.pop_back();
        return;
    }

    //create empty containers awaiting at port file for cases that port visit hasn't such file
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

        //don't run algorithms on defective travel
        else
        {
            std::cout << "\ttravel error" << std::endl;
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

    std::cout <<"\t===============================================================" << std::endl;
    std::cout <<"\tstarted simulate " << algorithmName << " on " << travelPath << std::endl;
    std::cout <<"\t===============================================================" << std::endl;

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

    std::cout << "\t\tReading route" << std::endl;
    string routePath = IO::firstFileWithExtensionInDirectory(travelPath, "route");
    IO::readShipRoute(routePath, route,simErrors);
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

    //simulation indicated that there is no travel error, so if algorithm indicates about error, he fails
    if(algErrors.hasTravelError())
    {
        algorithmsResults[algorithmName].addTravelResult(travelName, -1);
        simErrors.addError(Error::ALGORITHM_FALSE_TRAVEL_ERROR);
        IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
        std::cout << "\t\t" << algorithmName << " was terminated because of false travel error" << std::endl;
        return simErrors;
    }

    std::cout << "\t\tReading ship plan" << std::endl;
    string shipPlanPath = IO::firstFileWithExtensionInDirectory(travelPath, "ship_plan");
    IO::readShipPlan(shipPlanPath, ship.getShipPlan(),simErrors);
    if(simErrors.hasTravelError())
    {
        std::cout << "\t\tsimulation travel error" << std::endl;
        IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
        return simErrors;
    }

    algErrors.addErrors(algorithm->readShipPlan(shipPlanPath));

    //simulation indicated that there is no travel error, so if algoirthm indicates about error, he fails
    if(algErrors.hasTravelError())
    {
        algorithmsResults[algorithmName].addTravelResult(travelName, -1);
        simErrors.addError(Error::ALGORITHM_FALSE_TRAVEL_ERROR);
        IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
        std::cout << "\t\t" << algorithmName << " was terminated because of false travel error" << std::endl;
        return simErrors;
    }

    //iterate over ports and handle each one
    for (size_t i = 0; i < ports.size(); i++)
    {
        vector<Container> containers;
        vector<Container> badContainers;
        std::cout << "\t\t--------" <<"Ship Arrived to port " << ports[i] << "--------" << std::endl;
        const std::filesystem::path pathOfContainersAwaitingAtPortFile = travelPath + "/" + ports[i] + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".cargo_data";

        //there is no containers awaiting at port file. supply empty file to the algorithm
        if(std::filesystem::exists(pathOfContainersAwaitingAtPortFile) == 0)
        {
            std::cout << "\t\tNo containers awaiting at port input file for " << ports[i] << " for visiting number " << indexOfVisitAtPort[ports[i]] + 1 << std::endl;
            algErrors.addErrors(algorithm->getInstructionsForCargo(travelPath + "/" + "empty_containers_awaiting_at_port_file", pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".crane_instructions"));
        }

        //there is containers awaiting at port file. supply it to the algorithm
        else
        {
            IO::readContainerAwaitingAtPortFile(pathOfContainersAwaitingAtPortFile.string(),ship,containers,badContainers,simErrors);
            if (route.inLastStop() && (!containers.empty() || !badContainers.empty()))
            {
                simErrors.addError(Error::LAST_PORT_HAS_CONTAINERS_WARNING);
            }
            algErrors.addErrors(algorithm->getInstructionsForCargo(pathOfContainersAwaitingAtPortFile.string(), pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".crane_instructions"));
        }

        //algorithm shouldn't report travel error. If he does, he fails
        if(algErrors.hasTravelError())
        {
            algorithmsResults[algorithmName].addTravelResult(travelName, -1);
            simErrors.addError(Error::ALGORITHM_FALSE_TRAVEL_ERROR);
            IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
            std::cout << "\t\t" << algorithmName << " was terminated because of false travel error" << std::endl;
            return simErrors;
        }

        craneManagementAnswer = craneManagement.readAndExecuteInstructions(ship, pathOfOutputFilesForAlgorithmAndTravel.string() + "/" + ports[i] + "_" + std::to_string(indexOfVisitAtPort[ports[i]]+1) + ".crane_instructions");
        simErrors.addErrors(craneManagementAnswer.errors);

        if(craneManagementAnswer.errors != (int) Error::SUCCESS)
        {
            //algorithm write wrong command
            algorithmsResults[algorithmName].addTravelResult(travelName, -1);
            IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
            std::cout << "\t\t" << algorithmName << " was terminated because of invalid command" << std::endl;
            return simErrors;
        }
        totalOperations += craneManagementAnswer.numOfOperations;

        int checkForErrorsAfterPortResult = checkForErrorsAfterPort(ship, ports[i], craneManagementAnswer, route,containers, badContainers,simErrors);

        if(checkForErrorsAfterPortResult != (int) Error::SUCCESS)
        {
            //algorithm did something wrong at port
            algorithmsResults[algorithmName].addTravelResult(travelName, -1);
            IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);
            std::cout << "\t\t" << algorithmName << " was terminated because of wrong behavior" << std::endl;
            return simErrors;
        }

        indexOfVisitAtPort[ports[i]]++;
        std::cout << "\t\t-----------" <<"Ship left port " << ports[i] << "-----------" << std::endl;
        route.incrementCurrentPort();

    }

    algorithmsResults[algorithmName].addTravelResult(travelName, totalOperations);
    IO::writeErrorsOfTravelAndAlgorithm(simErrors,algErrors, outputPathOfErrorsFile);

    std::cout <<"\t================================================================" << std::endl;
    std::cout <<"\tfinished simulate " << algorithmName << " on " << travelPath << std::endl;
    std::cout <<"\t================================================================" << std::endl;

    return simErrors;

}

int Simulation::checkForErrorsAfterPort(Ship& ship, const string &port, CraneManagement::CraneManagementAnswer& answer,Route& route, vector<Container>& containers,vector<Container> &badContainers, Errors& errors) {

    int result = (int) Error::SUCCESS;
    // --------------- check all containers supposed to be unloaded were unloaded ---------------
    if (ship.portToContainers[port].size() > 0) {
        std::cout << "\t\t\tNot all of the containers with of this port destination were unloaded" << std::endl;
        errors.addError(Error::ALGORITHM_IGNORED_CONTAINER_SHOULD_BE_UNLOADED,std::to_string(ship.portToContainers[port].size()) + " containers of port " + port);
        result = -1;
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
            std::cout << "\t\tError: algorithm didn't reject bad container" << std::endl;
            errors.addError(Error::ALGORITHM_BAD_CONTAINER_WASNT_REJECT, container.getId());
            result = -1;
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
                        std::cout << "\t\tError: algorithm rejected not far container" << std::endl;
                        errors.addError(Error::ALGORITHM_INCORRECTLY_REJECTED_CONTAINER, id);
                        result = -1;
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
                    std::cout << "\t\tError: algorithm loaded container with destination as current port" << std::endl;
                    result = -1;
                }
                if (destIsntInNextStops){
                    std::cout << "\t\tError: algorithm loaded a container not in destination" << std::endl;
                    errors.addError(Error::ALGORITHM_NOT_ALLOWED_INSTRUCTION,id);
                    result = -1;
                }
                inLoaded = true;
                break;
            }
        }
        if (!inRejected && !inLoaded) {
            std::cout << "\t\tError: algorithm didn't review some waiting-in-port containers" << std::endl;
            errors.addError(Error::ALGORITHM_IGNORED_CONTAINER_SHOULD_BE_LOADED, container.getId());
            result = -1;
        }
    }

    // --------------- check if ship is empty in end of travel ---------------
    if (route.inLastStop()) {
        int amountOfContainers = ship.getAmountOfContainers() > 0;
        if (amountOfContainers > 0) {
            std::cout << "\t\tError: algorithm finished with nonempty ship" << std::endl;
            errors.addError(Error::SHIP_ISNT_EMPTY_IN_END_OF_TRAVEL,std::to_string(amountOfContainers));
            result = -1;
        }
    }

    return result;
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
        std::cout << "\t\tFatal error: missing travel_path argument!" << std::endl;
        return 1;
    }

    if(std::filesystem::exists(travelsPathToCheck) == 0)
    {
        std::cout << "\t\tFatal error: travel_path argument isn't valid!" << std::endl;
        return 1;
    }
    return 0;
}





