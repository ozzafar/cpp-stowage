//
// Created by Oz Zafar on 09/04/2020.
//

#include "NaiveAlgorithm.h"


using std::stringstream;
using std::string;
using std::list;

void NaiveAlgorithm::sortContainers(vector<Container*>& containers){
    std::sort(std::begin(containers), std::end(containers), [this](Container *a, Container* b) -> bool
    {
        if (portToIndexesInRoute.count(a->getDestinationPort())==0){
            portToIndexesInRoute[a->getDestinationPort()].push_back(INT32_MAX);
        }
        if (portToIndexesInRoute.count(b->getDestinationPort())==0){
            portToIndexesInRoute[b->getDestinationPort()].push_back(INT32_MAX);
        }
        return portToIndexesInRoute[a->getDestinationPort()][0] <= portToIndexesInRoute[b->getDestinationPort()][0];
    });
}

void NaiveAlgorithm::getLoadInstructions(const string &input_path, const string &output_path) {
    vector<Container*> containers = readContainerAwaitingAtPortFile(input_path);
    NaiveAlgorithm::sortContainers(containers);
    ShipPlan& shipPlan = ship->getShipPlan();
    int amount = containers.size(), index = 0, X = shipPlan.getPlanLength(), Y = shipPlan.getPlanWidth();
    for (int i = 0; i < X ; ++i) {
        for (int j = 0; j < Y; ++j) {
            ContainersPosition& containersPosition = shipPlan.getContainerPosition(i, j);
            while (containersPosition.howManyAvailiable()) {
                if (index < amount) {
                    if(containers[index]->getDestinationPort() != shipRoute->getCurrentPortName())
                    {
                        if (calculator.tryOperation((char) CraneOperation::LOAD, containers.at(index)->getWeight(), i, j) == WeightBalanceCalculator::APPROVED) {
                            string des = containers[index]->getDestinationPort().substr(0,5);
                            if (!shipRoute->portInNextStops(des)){
                                std::cout << "Warning: can't load container " << containers[index]->getId() << " because it's destination port " << containers[index]->getDestinationPort()<< " isn't in the next stops of the route" << std::endl;
                                writeOperation(output_path, CraneOperation::REJECT, containers[index]->getId(), -1, -1, -1);
                            }
                            else {
                                writeOperation(output_path, CraneOperation::LOAD, containers[index]->getId(), containersPosition.getTopFloorNumber()+1, i, j);
                                containersPosition.load(containers[index]->getId(), false);
                            }
                        }
                    }
                    else {
                        std::cout << "Warning: conatainer with same destination port as current port was waiting for load ignored" << std::endl;
                    }
                    index++;
                }
                else {
                    return;
                }
            }
        }
    }
    // most far containers will be rejected if there is no enough space
    for (; index < amount ; index++){
        if (shipRoute->portInNextStops(containers[index]->getDestinationPort())) {
            writeOperation(output_path, CraneOperation::REJECT, containers[index]->getId(), -1, -1, -1);
        } else {
            std::cout << "Warning: can't load container " << containers[index]->getId() << " because it's destination port " << containers[index]->getDestinationPort()<< " isn't in the next stops of the route" << std::endl;
        }
    }
}

void NaiveAlgorithm::getUnloadInstructions(const string& port, const string &output_path) {
    Container container;
    ShipPlan& shipPlan = ship->getShipPlan();
    int X = shipPlan.getPlanLength(), Y = shipPlan.getPlanWidth();
    for (int i = 0; i < X; ++i) {
        for (int j = 0; j < Y; ++j) {
            ContainersPosition &containersPosition = shipPlan.getContainerPosition(i, j);
            int lowestPlace = findLowestPlaceOfPortInPosition(port, containersPosition);
            if (lowestPlace != -1) {
                vector<string> containersToReturn;
                int iterations = containersPosition.getNumOfActiveFloors()-lowestPlace;
                for (int k = 0; k < iterations; k++) {
                    container = ship->getContainerOfId(containersPosition.getTop());
                    if (calculator.tryOperation((char) CraneOperation::UNLOAD, container.getWeight(), i, j) == WeightBalanceCalculator::APPROVED) {
                        writeOperation(output_path, CraneOperation::UNLOAD, container.getId(),containersPosition.getTopFloorNumber(), i, j);
                        if (container.getDestinationPort() != port) {
                            containersToReturn.push_back(container.getId());
                        }
                        containersPosition.unload(container.getId(), false);
                    } else{

                    }
                }
                for (auto& containerId : containersToReturn){
                    writeOperation(output_path, CraneOperation::LOAD, containerId,containersPosition.getTopFloorNumber()+1, i, j);
                    containersPosition.load(containerId, false);
                }
            }
        }
    }
}

int NaiveAlgorithm::findLowestPlaceOfPortInPosition(const string &port, ContainersPosition& position) {
    int i = 0;
    for( auto& containerId : position) {
        if (ship->containerIdToDestination(containerId) == port){
            return i;
        }
        i++;
    }
    return -1;
}

// input_path is Containers-awaiting-at-port file
void NaiveAlgorithm::getInstructionsForCargo(const string& port, const string &input_path, const string &output_path)  {
    getUnloadInstructions(port, output_path);
    if (!shipRoute->inLastStop() && !input_path.empty()){
        getLoadInstructions(input_path, output_path);
    }
    shipRoute->incrementCurrentPort();
}

string NaiveAlgorithm::getName() {
    return "Naive";
}

NaiveAlgorithm::~NaiveAlgorithm() {
    delete shipRoute;
    delete ship;
}

NaiveAlgorithm::NaiveAlgorithm() {

}


