//
// Created by Oz Zafar on 03/05/2020.
//

#ifndef CPP_STOWAGE_ERRORS_H
#define CPP_STOWAGE_ERRORS_H

#include <string>
#include <vector>
#include "Error.h"
#include "ErrorsIterator.h"

using std::string;

//#define RUNNING_ON_NOVA

class Errors {

private:
    int errors = (int)(Error::SUCCESS);

public:

    Errors(int errors);

    int getErrorsCode();

    bool hasError(Error error);

    bool hasError();

    bool hasTravelError();

    Errors getTravelErrors();

    void addError(Error newError);

    void addErrors(Errors newErrors);

    void addErrors(int newErrors);

    int emptyErrors();

    ErrorsIterator getIterator();

    static string errorToString(Error error);

    static int getCodeOfError(Error error);

    static string errorFromAlgorithmToString(Error error);

    void addError(int newError);

};
#endif //CPP_STOWAGE_ERRORS_H
