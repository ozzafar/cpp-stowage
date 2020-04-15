#include <iostream>
#include "common/Container.h"
#include "common/NaiveAlgorithm.h"
#include <list>
using std::list;



int main() {

    /*list<string> ls;

    ls.push_back("hello");
    std::cout << ls.front();
    std::cout << ls.size();*/
    Container container(0, "port1", "id");
    auto id =  container.getId();
    std::cout << id << std::endl;
    return 0;
}
