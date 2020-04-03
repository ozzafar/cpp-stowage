//
// Created by Oz Zafar on 28/03/2020.
//

#include "Container.h"

Container::Container(int weight, const string &destinationPort, const string &id) :
    weight(weight), destinationPort(destinationPort),id(id){
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

Container::~Container() {

}

