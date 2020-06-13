//
// Created by Oz Zafar on 09/05/2020.
//

#ifndef CPP_STOWAGE_ALGORITHMREGISTRATION_H
#define CPP_STOWAGE_ALGORITHMREGISTRATION_H


#pragma once

#include <functional>
#include <memory>
#include "../interfaces/AbstractAlgorithm.h"

class AlgorithmRegistration {
public:
    AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()>);
};


#define REGISTER_ALGORITHM(class_name) \
AlgorithmRegistration register_me_##class_name \
	([]{return std::make_unique<class_name>();} );



#endif //CPP_STOWAGE_ALGORITHMREGISTRATION_H
