//
// Created by Oz Zafar on 12/06/2020.
//

#include "Optimized1.h"
#include "../interfaces/AlgorithmRegistration.h"

#ifdef RUNNING_ON_NOVA
REGISTER_ALGORITHM (Optimized1)
#endif
int Optimized1::getUnloadInstructions(const string &output_path) {
    Errors errors;
    Container container;
    ShipPlan& shipPlan = ship.getShipPlan();
    string& port = route.getCurrentPortName();
    int X = shipPlan.getPlanLength(), Y = shipPlan.getPlanWidth();
    for (int i = 0; i < X; ++i) {
        for (int j = 0; j < Y; ++j) {
            ContainersPosition &containersPosition = shipPlan.getContainerPosition(i, j);
            int lowestPlace = findLowestPlaceOfPortInPosition(port, containersPosition);
            if (lowestPlace != -1) {
                vector<string> containersToReturn;
                int iterations = containersPosition.getNumOfActiveFloors()-lowestPlace;
                // starting to unload from top to bottom (lowestPlace)
                for (int k = 0; k < iterations; k++) {
                    container = ship.getContainerOfId(containersPosition.getTop()); // get top container
                    if (container.getDestinationPort() == port) {
                        writeOperation(output_path, Action::UNLOAD, container.getId(), containersPosition.getTopFloorNumber(), i, j);
                        containersPosition.unload(container.getId());
                    }
                    else{
                        moveOrTemporaryUnloadContainer(output_path, container.getId(), i, j, containersPosition,containersToReturn);
                    }
                }
                for (auto &containerId : containersToReturn) {
                    writeOperation(output_path, Action::LOAD, containerId, containersPosition.getTopFloorNumber() + 1,i, j);
                    containersPosition.load(containerId, false);
                }
            }
        }
    }
    return errors.getErrorsCode();
}


void Optimized1::moveOrTemporaryUnloadContainer(const string &output_path, string &containerId, int i,
                                                                  int j, ContainersPosition &containersPosition, vector<string> &containersToReturn) {
    ShipPlan& shipPlan = ship.getShipPlan();
    int targetX = -1 , targetY = -1, targetFloor = -1;

    // trying to find place to move the container into
    findFirstAvailablePosition(i, j,targetFloor,targetX, targetY);

    if (targetX != -1 && targetY!=-1) {
        // move
        writeOperation(output_path, Action::MOVE, containerId, containersPosition.getTopFloorNumber(), i, j, targetFloor, targetX, targetY);
        ContainersPosition& target = shipPlan.getContainerPosition(targetX,targetY);
        ContainersPosition::move(containersPosition,target,containerId);
    } else{
        // unload
        writeOperation(output_path, Action::UNLOAD, containerId, containersPosition.getTopFloorNumber(), i, j);
        containersPosition.unload(containerId);
        containersToReturn.push_back(containerId);
    }
}

void Optimized1::findFirstAvailablePosition(int i, int j, int & targetFloor, int &targetX, int &targetY)  {
    /* better to move to previous positions because the algorithm already
    * removed from them the relevant containers so it won't come back there */
    ShipPlan& shipPlan = ship.getShipPlan();
    int X = shipPlan.getPlanLength(), Y = shipPlan.getPlanWidth();
    for (int x = 0; x < X; ++x) {
        for (int y = 0; y < Y; ++y) {
            if (x!=i || y!=j){
                // not the same position
                if (shipPlan.getContainerPosition(x,y).howManyAvailiable()){
                    // there is available place to move into
                    targetX = x, targetY = y, targetFloor = shipPlan.getContainerPosition(x,y).getTopFloorNumber()+1;
                    return;
                }
            }
        }
    }
}

int Optimized1::getLoadInstructions(const string &input_path, const string &output_path) {
    Errors errors;
    vector<Container> containers;
    vector<Container> badContainers;
    errors.addErrors(readContainerAwaitingAtPortFile(input_path, containers,badContainers));
    NaiveAlgorithm::sortContainers(containers);
    ShipPlan& shipPlan = ship.getShipPlan();
    bool done;

    for (auto& container : badContainers){
        writeOperation(output_path, Action::REJECT, container.getId(), -1, -1, -1);
    }

    int floor = 0, numOfFloors = shipPlan.getFloors(), amount = containers.size(),
            index = 0, X = shipPlan.getPlanLength(), Y = shipPlan.getPlanWidth(),saveX = -1,saveY = -1;
    while (index < amount && floor < numOfFloors) {
        for (int i = 0; i < X; ++i) {
            for (int j = 0; j < Y; ++j) {
                ContainersPosition &containersPosition = shipPlan.getContainerPosition(i, j);
                if (containersPosition.howManyAvailiable() > 0) {
                    if (containers[index].getDestinationPort() != route.getCurrentPortName()) {
                        if (calculator.tryOperation((char) Action::LOAD, containers.at(index).getWeight(), i, j) ==
                            WeightBalanceCalculator::APPROVED) {
                            string des = containers[index].getDestinationPort().substr(0, 5);
                            if (!route.portInNextStops(des)) {
                                done = true;
                                writeOperation(output_path, Action::REJECT, containers[index].getId(), -1, -1, -1);
                            } else {
                                if (containersPosition.getNumOfActiveFloors()>0) {
                                    saveX = i, saveY = j;
                                    continue;
                                }
                                done = true;
                                writeOperation(output_path, Action::LOAD, containers[index].getId(),
                                               containersPosition.getTopFloorNumber() + 1, i, j);
                                containersPosition.load(containers[index].getId(), false);
                            }
                            if (index+1 < amount && containers[index].getDestinationPort()==containers[index+1].getDestinationPort()){
                                j--;
                            }
                        }
                    } else {
                        writeOperation(output_path, Action::REJECT, containers[index].getId(), -1, -1, -1);
                    }
                    finishIteration(index, saveX, saveY);
                    if (index==amount){
                        return errors.getErrorsCode();
                    }
                } else {
                    continue;
                }
            }
        }
        if (!done && saveX!=-1){
            ContainersPosition &containersPosition = shipPlan.getContainerPosition(saveX, saveY);
            writeOperation(output_path, Action::LOAD, containers[index].getId(),
                           containersPosition.getTopFloorNumber() + 1, saveX, saveY);
            containersPosition.load(containers[index].getId(), false);
            finishIteration(index, saveX, saveY);
            if (index==amount){
                return errors.getErrorsCode();
            }
        }
        done = false;
        floor++;
    }
    // most far containers will be rejected if there is no enough space
    for (; index < amount; index++) {
        if (route.portInNextStops(containers[index].getDestinationPort())) {
            errors.addError(Error::PASS_TOTAL_CONTAINERS_AMOUNT_LIMIT_WARNING);
            writeOperation(output_path, Action::REJECT, containers[index].getId(), -1, -1, -1);
        } else {
            std::cout << "Warning: can't load container " << containers[index].getId()
                      << " because it's destination port " << containers[index].getDestinationPort()
                      << " isn't in the next stops of the route" << std::endl;
        }
    }
    return errors.getErrorsCode();
}

void Optimized1::finishIteration(int &index, int &saveX, int &saveY) const {
    index++;
    saveX = -1, saveY = -1;
}
