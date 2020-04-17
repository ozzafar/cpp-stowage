//
// Created by Oz Zafar on 28/03/2020.
//

#ifndef CPP_STOWAGE_CONTAINER_H
#define CPP_STOWAGE_CONTAINER_H

#include <string>
#include <iostream>

using std::string ;

#define SUCCESS 1;
#define FAILURE 0;

class Container {
private:
    int weight;
    string destinationPort;
    string id;

public:
    Container();

    Container(int weight, const string &destinationPort, const string &id);

    int getWeight() const;

    string& getDestinationPort();

    const string &getId() const;

    // toString of container
    friend std::ostream& operator<<(std::ostream& out, const Container& c);

    virtual ~Container();
};


#endif //CPP_STOWAGE_CONTAINER_H
