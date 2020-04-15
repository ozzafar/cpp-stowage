#include <iostream>
#include "common/Container.h"
#include "common/NaiveAlgorithm.h"
#include <list>
using std::list;



int main() {

    Container container(0, "port1", "id");
    auto id =  container.getId();
    std::cout << id << std::endl;
    return 0;
}
