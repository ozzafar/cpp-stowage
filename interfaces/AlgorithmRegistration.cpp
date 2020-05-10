//
// Created by Oz Zafar on 10/05/2020.
//

#include "AlgorithmRegistration.h"
#include "../common/utils/Registrar.h"


AlgorithmRegistration::AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithm) {
    Registrar::getInstance().registerAlgorithm(algorithm);
}
