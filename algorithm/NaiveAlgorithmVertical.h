//
// Created by Oz Zafar on 13/05/2020.
//

#ifndef CPP_STOWAGE_NAIVEALGORITHMVERTICAL_H
#define CPP_STOWAGE_NAIVEALGORITHMVERTICAL_H


#include "NaiveAlgorithm.h"

class NaiveAlgorithmVertical : public NaiveAlgorithm {
     int getLoadInstructions(const string &input_path, const string &output_path) override;

};


#endif //CPP_STOWAGE_NAIVEALGORITHMVERTICAL_H
