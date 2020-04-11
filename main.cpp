#include <iostream>
#include "common/Container.h"
#include "common/Algorithm.h"
#include <list>
using std::list;



int main() {

    /*list<string> ls;

    ls.push_back("hello");
    std::cout << ls.front();
    std::cout << ls.size();*/
    Container container(0, "port1", "id");
    Algorithm algorithm;
    algorithm.readShipPlan("C:/ship_plan.txt");
    algorithm.readShipRoute("C:/route.txt");
    auto id =  container.getId();
    id.at(0)='x';

    return 0;
}
