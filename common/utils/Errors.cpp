//
// Created by Oz Zafar on 03/05/2020.
//

#include <cmath>
#include <iostream>
#include "Errors.h"

static const string ALGORITHM_TRUE_NEGATIVE_ERROR = "The algorithm didn't report on the error: ";


string Errors::errorToString(Error error) {
    switch (error) {
        // Ship Plan
        case Error::NUMBER_OF_FLOORS_WARNING:
            return "ship plan: a position has an equal number of floors, or more, than the number of floors provided in the first line (ignored)";
        case Error::ILLEGAL_POSITION_WARNING:
            return "ship plan: a given position exceeds the X/Y ship limits (ignored)";
        case Error::PLAN_BAD_LINE_FORMAT_WARNING:
            return "ship plan: bad line format after first line (ignored)";
        case Error::PLAN_FILE_CANNOT_BE_READ_ERROR:
            return "ship plan: fatal error - bad first line or file cannot be read altogether (cannot run with this ship plan)";
        // Reserved Code
        case Error::RESERVED1:
            return "reserved";
        case Error::RESERVED2:
            return "reserved";
        // Route Code
        case Error::PORT_APPEAR_TWICE_WARNING:
            return "travel route: a port appears twice or more consecutively (ignored)";
        case Error::BAD_PORT_SYMBOL_WARNING:
            return "travel route: bad port symbol format (ignored)";
        case Error::ROUTE_FILE_CANNOT_BE_READ_ERROR:
            return "travel route: fatal error - empty file or file cannot be read altogether (cannot run this travel)";
        case Error::ROUTE_FILE_SINGLE_VALID_PORT_ERROR:
            return "travel route: fatal error - file with only a single valid port (cannot run this travel)";
        // Container Code
        case Error::DUPLICATE_CONTAINER_ID_WARINING:
            return "containers at port: duplicate ID on port (ID rejected)";
        case Error::CONTAINER_ID_ALREADY_IN_SHIP_WARINING:
            return "containers at port: ID already on ship (ID rejected)";
        case Error::MISSING_OR_BAD_WEIGHT_WARINING:
            return "containers at port: bad line format, missing or bad weight (ID rejected)";
        case Error::MISSING_OR_BAD_DEST_WARINING:
            return "containers at port: bad line format, missing or bad port dest (ID rejected)";
        case Error::ID_CANNOT_BE_READ_WARINING:
            return "bad line format, ID cannot be read (ignored)";
        case Error::ILLEGAL_ID_CHECK_WARNING:
            return "illegal ID check ISO 6346 (ID rejected)";
        case Error::CONTAINER_FILE_CANNOT_BE_READ_WARNING:
            return "file cannot be read altogether (assuming no cargo to be loaded at this port)";
        case Error::LAST_PORT_HAS_CONTAINERS_WARNING:
            return "last port has waiting containers (ignored)";
        case Error::PASS_TOTAL_CONTAINERS_AMOUNT_LIMIT_WARNING:
            return "total containers amount exceeds ship capacity (rejecting far containers)";
        case Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT:
            return "can't load container with destination port as the current port";
        // Container Position Code
        case Error::FULL_CONTAINER_POSITION:
            return "can't load container to a full position";
        case Error::UNLOAD_NOT_TOP_CONTAINER:
            return "can't load container to a full position";
        default:
            return "";
    }
}


string Errors::errorFromAlgorithmToString(Error error) {
    return ALGORITHM_TRUE_NEGATIVE_ERROR + errorToString(error);
}

int Errors::getCodeOfError(Error error) {
    return log2((int) error);
}

int Errors::getErrorsCode() {
    return errors;
}

bool Errors::hasError(Error error) {
    return errors && (int) error;
}


bool Errors::hasFatalError() {
    int fatalErrorCode = (int) Error::PLAN_FILE_CANNOT_BE_READ_ERROR | (int) Error::ROUTE_FILE_CANNOT_BE_READ_ERROR |
                         (int) Error::ROUTE_FILE_SINGLE_VALID_PORT_ERROR;
    return errors && fatalErrorCode;;
}

void Errors::addErrors(Errors newErrors) {
    addErrors(newErrors.getErrorsCode());
}

void Errors::addErrors(int newErrors) {
    int error = 1; // 2 pow 0
    while (newErrors) {
        if (newErrors & 1) { // check if first but is 1 ==> error exists
            addError(error);
        }
        error *= 2;
        newErrors /= 2;
    }
}

void Errors::addError(int newError) {
    errors |= newError;
}

void Errors::addError(Error newError) {
    addError((int) newError);
}

bool Errors::hasError() {
    return errors!=(int)Error::SUCCESS;
}

int Errors::emptyErrors() {
    return errors = (int)Error::SUCCESS;
}

ErrorsIterator Errors::getIterator() {
    return ErrorsIterator(errors);
}
