//
// Created by Oz Zafar on 28/03/2020.
//

#ifndef CPP_PROJECTS_CONTAINER_H
#define CPP_PROJECTS_CONTAINER_H

#include <string>
using namespace std;

class Container {
private:
    int weight;
    string destinationPort;
    string id;

public:

    Container(int weight, const string &destinationPort, const string &id);

    int getWeight() const;

    const string &getDestinationPort() const;

    const string &getId() const;
};


#endif //CPP_PROJECTS_CONTAINER_H
