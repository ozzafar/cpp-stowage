#include <iostream>
#include "common/Container.h"

int main() {
    Container container(0, "port1", "id");
    string id =  container.getId();
    std::cout << id << std::endl;
    return 0;
}
