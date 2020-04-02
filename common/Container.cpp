//
// Created by Oz Zafar on 28/03/2020.
//

#include <iostream>
#include "Container.h"

Container::Container(int weight, const string &destinationPort, const string &id) : weight(weight) {
    if (destinationPort.length() != 5) {
        std::cout << "Error: destination port is illegal" << std::endl;
    } else {
        this->destinationPort = destinationPort;
        this->id = id;
    }
}

int Container::getWeight() const {
    return weight;
}

const string &Container::getDestinationPort() const {
    return destinationPort;
}

const string &Container::getId() const {
    return id;
}

