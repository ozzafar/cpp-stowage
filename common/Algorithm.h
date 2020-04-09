//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_ALGORITHM_H
#define CPP_STOWAGE_ALGORITHM_H

#include <string>
using std::string;

class Algorithm {
    void readShipPlan(const string& full_path_and_file_name);
    void readShipRoute(const string& full_path_and_file_name);
};


#endif //CPP_STOWAGE_ALGORITHM_H
