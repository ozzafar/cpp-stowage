//
// Created by Oz Zafar on 09/04/2020.
//

#ifndef CPP_STOWAGE_ROUTE_H
#define CPP_STOWAGE_ROUTE_H

#include <list>
#include <string>
#include <utility>
#include <map>

using std::list;
using std::string;
using std::map;

class Route {
private:
    list<string> ports;
public:
    Route();
    Route(list<string> ports) : ports(std::move(ports)){}

    const list<string> &getPorts() const;


};


#endif //CPP_STOWAGE_ROUTE_H
