//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_ROUTE_H
#define CPP_STOWAGE_ROUTE_H

#include <list>
#include <string>

using std::list;
using std::string;

class Route {
private:
    list<string> ports;
public:
    Route();
    Route(const list<string> &ports);
};


#endif //CPP_STOWAGE_ROUTE_H
