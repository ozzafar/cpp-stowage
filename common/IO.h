//
// Created by Oz Zafar on 18/04/2020.
//

#ifndef CPP_STOWAGE_IO_H
#define CPP_STOWAGE_IO_H


#include "../src/Container.h"
#include "../src/Ship.h"
#include "../src/Route.h"
#include <vector>
#include <stdio.h>
#include <ctype.h>

using std::vector;

class IO {

public:

    static vector<Container *> readContainerAwaitingAtPortFile(const string &input_path, Ship *ship);

    static vector<string> breakLineToWords(string &line, char delimeter);

    static void readShipPlan(const string &path, Ship *ship);

    static Route* readShipRoute(const string &path);

};

#endif //CPP_STOWAGE_IO_H
