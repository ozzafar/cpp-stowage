//
// Created by Oz Zafar on 28/03/2020.
//

#include <iostream>
#include "Container.h"


int Container::getWeight() const {
    return weight;
}

const string Container::getDestinationPort() const {
    return destinationPort;
}

string Container::getId() const {
    return id;
}

Container::Container(int weight, string destinationPort, string id) : weight(weight) {
    if (destinationPort.length()!=5){
        std::cout << "Error: destination port is illegal" << std::endl;
    } else {
        //this->destinationPort = new char[5];
        this->id = id;
    }
}


