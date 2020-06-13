//
// Created by Oz Zafar on 13/05/2020.
//

#include "NaiveAlgorithmHorizontal.h"
#include "../common/utils/Errors.h"

int NaiveAlgorithmHorizontal::getLoadInstructions(const string &input_path, const string &output_path) {
    Errors errors;
    vector<Container> containers;
    vector<Container> badContainers;
    errors.addErrors(readContainerAwaitingAtPortFile(input_path, containers,badContainers));
    NaiveAlgorithm::sortContainers(containers);
    ShipPlan& shipPlan = ship.getShipPlan();

    for (auto& container : badContainers){
        writeOperation(output_path, Action::REJECT, container.getId(), -1, -1, -1);
    }

    int floor = 0, numOfFloors = shipPlan.getFloors(), amount = containers.size(), index = 0, X = shipPlan.getPlanLength(), Y = shipPlan.getPlanWidth();
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
                                writeOperation(output_path, Action::REJECT, containers[index].getId(), -1, -1, -1);
                            } else {
                                writeOperation(output_path, Action::LOAD, containers[index].getId(),
                                               containersPosition.getTopFloorNumber() + 1, i, j);
                                containersPosition.load(containers[index].getId(), false);
                            }
                        }
                    } else {
                        writeOperation(output_path, Action::REJECT, containers[index].getId(), -1, -1, -1);
                    }
                    index++;
                    if (index==amount){
                        return errors.getErrorsCode();
                    }
                } else {
                    continue;
                }
            }
        }
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
