//
// Created by Oz Zafar on 09/04/2020.
//

#include "NaiveAlgorithm.h"



using std::stringstream;
using std::string;
using std::list;

// region HELP FUNCTIONS

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
// endregion

// region INSTRUCTIONS

// input_path is Containers-awaiting-at-port file
int NaiveAlgorithm::getInstructionsForCargo(const string &input_path, const string &output_path)  {
    std::ofstream outfile (output_path);
    outfile.close();
    Errors errors;
    errors.addErrors(getUnloadInstructions(output_path));
    if (!route.inLastStop() && !input_path.empty()){
        errors.addErrors(getLoadInstructions(input_path, output_path));
    }
    route.incrementCurrentPort();
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

// endregion


NaiveAlgorithm::NaiveAlgorithm() {
}



