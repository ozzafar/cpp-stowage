//
// Created by Oz Zafar on 10/05/2020.
//

#include "../interfaces/AlgorithmRegistration.h"
#include "../common/utils/Registrar.h"


AlgorithmRegistration::AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithm) {
    std::cout <<"entered algorithm registration" << std::endl;
    Registrar::getInstance().registerAlgorithm(algorithm);
}
