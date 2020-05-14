//
// Created by Oz Zafar on 14/05/2020.
//

#ifndef CPP_STOWAGE_ERRORSITERATOR_H
#define CPP_STOWAGE_ERRORSITERATOR_H


#include "Errors.h"

class ErrorsIterator {
    int errors;
    int index;

public:
    ErrorsIterator(int errors);

private:
    bool hasNext();
    Error getNext();
};


#endif //CPP_STOWAGE_ERRORSITERATOR_H
