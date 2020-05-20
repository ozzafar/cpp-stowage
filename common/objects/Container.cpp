//
// Created by Oz Zafar on 28/03/2020.
//

#include "Container.h"
#include "../utils/ISO_6346.h"
#include <map>

Container::Container(int weight, const string &destinationPort, const string &id) :
    weight(weight), destinationPort(destinationPort),id(id){
}

int Container::getWeight() const {
    return weight;
}

string& Container::getDestinationPort()  {
    return destinationPort;
}

string &Container::getId() {
    return id;
}

std::ostream &operator<<(std::ostream &out, const Container &c) {
    return out << "id: " << c.id << " destination: " << c.destinationPort << " weight: " << c.weight;
}

Container::Container() {}

bool Container::isValidId(const string& id)  {
    return ISO_6346::isValidId(id);
}
