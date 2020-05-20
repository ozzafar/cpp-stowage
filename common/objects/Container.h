//
// Created by Oz Zafar on 28/03/2020.
//

#ifndef CPP_STOWAGE_CONTAINER_H
#define CPP_STOWAGE_CONTAINER_H

#include <string>
#include <iostream>

using std::string ;

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

    string &getId();

    // toString of container
    friend std::ostream& operator<<(std::ostream& out, const Container& c);

    static bool isValidId(const string& id);
};


#endif //CPP_STOWAGE_CONTAINER_H
