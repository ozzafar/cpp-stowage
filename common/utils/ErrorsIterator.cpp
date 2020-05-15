//
// Created by Oz Zafar on 14/05/2020.
//

#include "ErrorsIterator.h"

Error convertToError(int errorCode) {
    return (Error)(1<<errorCode);
}

bool ErrorsIterator::hasNext() {
    int copyErrors = errors;
    while (copyErrors){
        if (copyErrors & 1){
            return true;
        }
        copyErrors/=2;
    }
    return false;
}

Error ErrorsIterator::getNext() {
    Error err = Error::SUCCESS;
    while (errors){
        if (errors & 1){
            err = convertToError(index);
            index++;
            errors/=2;
            return err;
        }
        index++;
        errors/=2;
    }
    return err;
}

ErrorsIterator::ErrorsIterator(int errors) {
    this->errors = errors;
    this->index = 0;
}



