//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_ROUTE_H
#define CPP_STOWAGE_ROUTE_H

#include <string>
#include <utility>

using std::string;
using std::vector;

class Route {

private:

    vector<string> ports;

    int currentPort = 0;

public:

    Route();

    Route(vector<string> ports);

    void incrementCurrentPort();

    int getCurrentPort() const;

    bool portInNextStops(string& port);

    const list<string> &getPorts() const;

};


#endif //CPP_STOWAGE_ROUTE_H
