//
// Created by Oz Zafar on 09/04/2020.
//

#include <vector>
#include "Route.h"

Route::Route(vector<string> ports) : ports(ports) {}

Route::Route() {}

int Route::getCurrentPortIndex() const {
    return currentPort;
}

void Route::incrementCurrentPort() {
    currentPort+=1;

}

bool Route::portInNextStops(string &port) {
    for (size_t i = currentPort; i < ports.size() ; i++) {
        if (ports.at(i) == port){
            return true;
        }
    }
    return false;
}

const vector<string> &Route::getPorts() const {
    return ports;
}

bool Route::inLastStop() {
    return (size_t) currentPort == ports.size()-1;
}

int Route::nextStopForPort(string& port) {
    for (size_t i = currentPort ; i < ports.size() ; i++){
        if (ports[i]==port){
            return i;
        }
    }
    return INT32_MAX;
}

string& Route::getCurrentPortName() {
    return ports.at(currentPort);
}

