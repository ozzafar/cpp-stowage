//
// Created by Oz Zafar on 13/05/2020.
//

#ifndef CPP_STOWAGE_NAIVEALGORITHMHORIZONTAL_H
#define CPP_STOWAGE_NAIVEALGORITHMHORIZONTAL_H


#include "NaiveAlgorithm.h"

class NaiveAlgorithmHorizontal : public NaiveAlgorithm {
     int getLoadInstructions(const string &input_path, const string &output_path) override;

};


#endif //CPP_STOWAGE_NAIVEALGORITHMHORIZONTAL_H
