//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_WEIGHTBALANCECALCULATOR_H
#define CPP_STOWAGE_WEIGHTBALANCECALCULATOR_H

#pragma once

class WeightBalanceCalculator {
public:

    enum BalanceStatus {
        APPROVED, X_IMBALANCED, Y_IMBALANCED, X_Y_IMBALANCED
    };

    int readShipPlan(const std::string& full_path_and_file_name);

    BalanceStatus tryOperation(char loadUnload, int kg, int x, int y);
};



#endif //CPP_STOWAGE_WEIGHTBALANCECALCULATOR_H
