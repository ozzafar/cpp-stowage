//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_WEIGHTBALANCECALCULATOR_H
#define CPP_STOWAGE_WEIGHTBALANCECALCULATOR_H


#include "../common/objects/ShipPlan.h"

class WeightBalanceCalculator {

public:
    enum BalanceStatus {
        APPROVED, X_IMBALANCED, Y_IMBALANCED, X_Y_IMBALANCED
    };

    BalanceStatus tryOperation(char loadUnload, int kg, int x, int y);
};


#endif //CPP_STOWAGE_WEIGHTBALANCECALCULATOR_H
