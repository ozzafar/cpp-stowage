#include <iostream>
#include "common/Container.h"

int main() {
    Container container(0, "port1", "id");
    string id =  container.getId();
    id.at(0)='x';
    std::cout << id << std::endl;
    std::cout << container.getId() << std::endl;
    return 0;
}
