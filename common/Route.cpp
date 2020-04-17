//
// Created by Oz Zafar on 09/04/2020.
//

#include <vector>
#include "Route.h"

Route::Route(vector<string> ports) : ports(ports) {}

Route::Route() {}

int Route::getCurrentPort() const {
    return currentPort;
}

void Route::incrementCurrentPort() {
    currentPort+=1;

}

bool Route::portInNextStops(string &port) {
    for (int i = currentPort; i < ports.size() ; i++) {
        if (ports.at(i) == port){
            return true;
        }
    }
    return false;
}

bool Route::inLastStop() {
    return currentPort == ports.size();
}

