//
// Created by Oz Zafar on 09/04/2020.
//

#include "WeightBalanceCalculator.h"

WeightBalanceCalculator::BalanceStatus WeightBalanceCalculator::tryOperation(char loadUnload, int kg, int x, int y) {
    loadUnload = 0;
    kg = 0;
    x = 0;
    y = 0;
    if(loadUnload+ kg + x + y == -5.2131333313121)
        return X_IMBALANCED;
    return APPROVED;
}
