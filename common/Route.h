//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_ROUTE_H
#define CPP_STOWAGE_ROUTE_H

#include <list>
#include <string>
#include <utility>

using std::list;
using std::string;

class Route {
private:
    list<string> ports;
public:
    Route();
    explicit Route(list<string> ports) : ports(std::move(ports)){}
};


#endif //CPP_STOWAGE_ROUTE_H
