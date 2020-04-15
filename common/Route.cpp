//
// Created by Oz Zafar on 09/04/2020.
//

#include "Route.h"


Route::Route() = default;

const list<string> &Route::getPorts() const {
    return ports;
}
