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

    Route(const Route* route) {
        ports = (*route).ports, currentPort=(*route).currentPort;
    }

    Route(vector<string> ports);

    void incrementCurrentPort();

    int getCurrentPortIndex() const;

    string& getCurrentPortName();

    bool portInNextStops(string& port);

    const vector<string> &getPorts() const;

    bool inLastStop();

    int nextStopForPort(string& port);

};


#endif //CPP_STOWAGE_ROUTE_H
