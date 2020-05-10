//
// Created by Oz Zafar on 09/04/2020.
//

#include "NaiveAlgorithm.h"
#include "../common/utils/Errors.h"
#include "../interfaces/AlgorithmRegistration.h"


using std::stringstream;
using std::string;
using std::list;

void NaiveAlgorithm::sortContainers(vector<Container>& containers){
    std::sort(std::begin(containers), std::end(containers), [this](  auto& a,   auto& b) -> bool
    {
        if (portToIndexesInRoute.count(a.getDestinationPort())==0){
            portToIndexesInRoute[a.getDestinationPort()].push_back(INT32_MAX);
        }
        if (portToIndexesInRoute.count(b.getDestinationPort())==0){
            portToIndexesInRoute[b.getDestinationPort()].push_back(INT32_MAX);
        }
        return portToIndexesInRoute[a.getDestinationPort()][0] < portToIndexesInRoute[b.getDestinationPort()][0];
    });
}

int NaiveAlgorithm::getLoadInstructions(const string &input_path, const string &output_path) {
    Errors errors;
    vector<Container> containers;
    errors.addErrors(readContainerAwaitingAtPortFile(input_path,containers));
    NaiveAlgorithm::sortContainers(containers);
    ShipPlan& shipPlan = ship.getShipPlan();
    int amount = containers.size(), index = 0, X = shipPlan.getPlanLength(), Y = shipPlan.getPlanWidth();
    for (int i = 0; i < X ; ++i) {
        for (int j = 0; j < Y; ++j) {
            ContainersPosition& containersPosition = shipPlan.getContainerPosition(i, j);
            while (containersPosition.howManyAvailiable()) {
                if (index < amount) {
                    if(containers[index].getDestinationPort() != route.getCurrentPortName())
                    {
                        if (calculator.tryOperation((char) Action::LOAD, containers.at(index).getWeight(), i, j) == WeightBalanceCalculator::APPROVED) {
                            string des = containers[index].getDestinationPort().substr(0,5);
                            if (!route.portInNextStops(des)){
                                //errors.addError();
                                std::cout << "Warning: can't load container " << containers[index].getId() << " because it's destination port " << containers[index].getDestinationPort()<< " isn't in the next stops of the route" << std::endl;
                                writeOperation(output_path, Action::REJECT, containers[index].getId(), -1, -1, -1);
                            }
                            else {
                                writeOperation(output_path, Action::LOAD, containers[index].getId(), containersPosition.getTopFloorNumber()+1, i, j);
                                containersPosition.load(containers[index].getId(), false);
                            }
                        }
                    }
                    else {
                        std::cout << "Warning: conatainer with same destination port as current port was waiting for load ignored" << std::endl;
                    }
                    index++;
                }
                else {
                    return errors.getErrorsCode();
                }
            }
        }
    }
    // most far containers will be rejected if there is no enough space
    for (; index < amount ; index++){
        if (route.portInNextStops(containers[index].getDestinationPort())) {
            errors.addError(Error::PASS_TOTAL_CONTAINERS_AMOUNT_LIMIT_WARNING);
            writeOperation(output_path, Action::REJECT, containers[index].getId(), -1, -1, -1);
        } else {
            std::cout << "Warning: can't load container " << containers[index].getId() << " because it's destination port " << containers[index].getDestinationPort()<< " isn't in the next stops of the route" << std::endl;
        }
    }
    return errors.getErrorsCode();
}

int NaiveAlgorithm::getUnloadInstructions(const string &output_path) {
    Errors errors;
    Container container;
    ShipPlan& shipPlan = ship.getShipPlan();
    string port = route.getCurrentPortName();
    int X = shipPlan.getPlanLength(), Y = shipPlan.getPlanWidth();
    for (int i = 0; i < X; ++i) {
        for (int j = 0; j < Y; ++j) {
            ContainersPosition &containersPosition = shipPlan.getContainerPosition(i, j);
            int lowestPlace = findLowestPlaceOfPortInPosition(port, containersPosition);
            if (lowestPlace != -1) {
                vector<string> containersToReturn;
                int iterations = containersPosition.getNumOfActiveFloors()-lowestPlace;
                for (int k = 0; k < iterations; k++) {
                    container = ship.getContainerOfId(containersPosition.getTop());
                    if (calculator.tryOperation((char) Action::UNLOAD, container.getWeight(), i, j) == WeightBalanceCalculator::APPROVED) {
                        writeOperation(output_path, Action::UNLOAD, container.getId(),containersPosition.getTopFloorNumber(), i, j);
                        if (container.getDestinationPort() != port) {
                            containersToReturn.push_back(container.getId());
                        }
                        containersPosition.unload(container.getId(), false);
                    } else{

                    }
                }
                for (auto& containerId : containersToReturn){
                    writeOperation(output_path, Action::LOAD, containerId,containersPosition.getTopFloorNumber()+1, i, j);
                    containersPosition.load(containerId, false);
                }
            }
        }
    }
    return errors.getErrorsCode();
}

int NaiveAlgorithm::findLowestPlaceOfPortInPosition(const string &port, ContainersPosition& position) {
    int i = 0;
    for( auto& containerId : position) {
        if (ship.containerIdToDestination(containerId) == port){
            return i;
        }
        i++;
    }
    return -1;
}

// input_path is Containers-awaiting-at-port file
int NaiveAlgorithm::getInstructionsForCargo(const string &input_path, const string &output_path)  {
    Errors errors;
    errors.addErrors(getUnloadInstructions(output_path));
    if (!route.inLastStop() && !input_path.empty()){
        errors.addErrors(getLoadInstructions(input_path, output_path));
    }
    route.incrementCurrentPort();
    return errors.getErrorsCode(); // TODO fix this
}


NaiveAlgorithm::NaiveAlgorithm() {
}



