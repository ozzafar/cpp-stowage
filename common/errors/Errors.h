//
// Created by Oz Zafar on 03/05/2020.
//

#ifndef CPP_STOWAGE_ERRORS_H
#define CPP_STOWAGE_ERRORS_H

#define SUCCESS 0
#include <string>
#include <vector>

using std::string;
using std::vector;


enum Error{

    // Ship Plan Code
    NUMBER_OF_FLOORS_WARNING                   = 1 << 0,
    ILLEGAL_POSITION_WARNING                   = 1 << 1,
    PLAN_BAD_LINE_FORMAT_WARNING                    = 1 << 2,
    PLAN_FILE_CANNOT_BE_READ_ERROR             = 1 << 3,

    // Reserved Code
    RESERVED1                                  = 1 << 4,
    RESERVED2                                  = 1 << 9,

    // Route Code
    PORT_APPEAR_TWICE_WARNING                  = 1 << 5,
    BAD_PORT_SYMBOL_WARNING                    = 1 << 6,
    ROUTE_FILE_CANNOT_BE_READ_ERROR            = 1 << 7,
    ROUTE_FILE_SINGLE_VALID_PORT_ERROR         = 1 << 8,

    // Container Code
    DUPLICATE_CONTAINER_ID_WARINING            = 1 << 10,
    CONTAINER_ID_ALREADY_IN_SHIP_WARINING      = 1 << 11,
    MISSING_OR_BAD_WEIGHT_WARINING             = 1 << 12,
    MISSING_OR_BAD_DEST_WARINING               = 1 << 13,
    ID_CANNOT_BE_READ_WARINING                 = 1 << 14,
    ILLEGAL_ID_CHECK_WARNING                   = 1 << 15,
    CONTAINER_FILE_CANNOT_BE_READ_WARNING      = 1 << 16,
    LAST_PORT_HAS_CONTAINERS_WARNING           = 1 << 17,
    PASS_TOTAL_CONTAINERS_AMOUNT_LIMIT_WARNING = 1 << 18,

};


class Errors {

private:
    int errors = SUCCESS;
    int errorsCounter[32];

public:
    int getErrorsCode();
    bool hasError(Error error);
    bool hasFatalError();
    void addError(Error newError);
    void addErrors(Errors newErrors);
    void addErrors(int newErrors);
    int amountOfError(Error error);

    static string errorToString(Error error);
    static int getCodeOfError(Error error);
};

#endif //CPP_STOWAGE_ERRORS_H
