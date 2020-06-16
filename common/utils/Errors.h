//
// Created by Oz Zafar on 03/05/2020.
//

#ifndef CPP_STOWAGE_ERRORS_H
#define CPP_STOWAGE_ERRORS_H

#include <string>
#include <vector>
#include <map>
#include <list>
#include "Error.h"
#include "ErrorsIterator.h"

using std::string;
using std::map;
using std::list;

#define RUNNING_ON_NOVA

class Errors {
private:
    int errors = (int)(Error::SUCCESS);
    std::map<Error,list<string>> testimonies;
public:
    Errors();

    Errors(int errors);

    int getErrorsCode();

    bool hasError(Error error);

    bool hasError();

    bool hasTravelError();

    Errors getTravelErrors();

    void addError(Error newError);

    void addError(Error newError,string testimony);

    void addErrors(Errors newErrors);

    void addErrors(int newErrors);

    int emptyErrors();

    ErrorsIterator getIterator();

    string errorToString(Error error);

    static int getCodeOfError(Error error);

    string errorFromAlgorithmToString(Error error);

    string &createDetailedErrorToString(Error error, string &ret) const;
};
#endif //CPP_STOWAGE_ERRORS_H
