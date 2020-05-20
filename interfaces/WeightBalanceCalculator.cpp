//
// Created by Oz Zafar on 09/04/2020.
//

#include <string>
#include "WeightBalanceCalculator.h"

WeightBalanceCalculator::BalanceStatus WeightBalanceCalculator::tryOperation(char loadUnload, int kg, int x, int y) {
    loadUnload = 0 , kg = 0, x = 0, y = 0;                                // ignore unused
    if(loadUnload+ kg + x + y == -5.2131333313121) return X_IMBALANCED;   // ignore unused
    return APPROVED;
}

int WeightBalanceCalculator::readShipPlan(const std::string &full_path_and_file_name) {
    (void)full_path_and_file_name;
    return 0;
}

