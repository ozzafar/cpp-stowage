//
// Created by Oz Zafar on 03/05/2020.
//

#include <cmath>
#include "Errors.h"

static const string ALGORITHM_TRUE_NEGATIVE_ERROR = "algorithm reported on the error - ";


string Errors::errorToString(Error error) {
    string tstms = "", ret;
    int i = 1;
    if (testimonies.count(error)>0){
        for (auto& tst : testimonies[error]){
            tstms += (std::to_string(i)+") " + tst+'\n');
            i++;
        }
    }
    switch (error) {
        // Ship Plan
        case Error::NUMBER_OF_FLOORS_WARNING:
            ret = "ship plan: a position has an equal number of floors, or more, than the number of floors provided in the first line (ignored)";
            break;
        case Error::ILLEGAL_POSITION_WARNING:
            ret = "ship plan: a given position exceeds the X/Y ship limits (ignored)";
            break;
        case Error::PLAN_BAD_LINE_FORMAT_WARNING:
            ret = "ship plan: bad line format after first line or duplicate x,y appearance with same data (ignored)";
            break;
        case Error::PLAN_FILE_CANNOT_BE_READ_ERROR:
            ret = "ship plan: fatal error - bad first line or file cannot be read altogether (cannot run with this ship plan)";
            break;
        case Error::DUPLICATE_X_Y_WITH_DIFFERENT_DATA:
            ret = "ship plan: travel error - duplicate x,y appearance with different data (cannot run this travel)";
            break;
            // Reserved Code
        case Error::RESERVED2:
            ret = "reserved";
            break;
            // Route Code
        case Error::PORT_APPEAR_TWICE_WARNING:
            ret = "travel route: a port appears twice or more consecutively (ignored)";
            break;
        case Error::BAD_PORT_SYMBOL_WARNING:
            ret = "travel route: bad port symbol format (ignored)";
            break;
        case Error::ROUTE_FILE_CANNOT_BE_READ_ERROR:
            ret = "travel route: fatal error - empty file or file cannot be read altogether (cannot run this travel)";
            break;
        case Error::ROUTE_FILE_SINGLE_VALID_PORT_ERROR:
            ret = "travel route: fatal error - file with only a single valid port (cannot run this travel)";
            break;
            // Container Code
        case Error::DUPLICATE_CONTAINER_ID_WARINING:
            ret = "containers at port: duplicate ID on port (ID rejected)";
            break;
        case Error::CONTAINER_ID_ALREADY_IN_SHIP_WARINING:
            ret = "containers at port: ID already on ship (ID rejected)";
            break;
        case Error::MISSING_OR_BAD_WEIGHT_WARINING:
            ret = "containers at port: bad line format, missing or bad weight (ID rejected)";
            break;
        case Error::MISSING_OR_BAD_DEST_WARINING:
            ret = "containers at port: bad line format, missing or bad port dest (ID rejected)";
            break;
        case Error::ID_CANNOT_BE_READ_WARINING:
            ret = "bad line format, ID cannot be read (ignored)";
            break;
        case Error::ILLEGAL_ID_CHECK_WARNING:
            ret = "illegal ID check ISO 6346 (ID rejected)";
            break;
        case Error::CONTAINER_FILE_CANNOT_BE_READ_WARNING:
            ret = "file cannot be read altogether (assuming no cargo to be loaded at this port)";
            break;
        case Error::LAST_PORT_HAS_CONTAINERS_WARNING:
            ret = "last port has waiting containers (ignored)";
            break;
        case Error::PASS_TOTAL_CONTAINERS_AMOUNT_LIMIT_WARNING:
            ret = "total containers amount exceeds ship capacity (rejecting far containers)";
            break;

        // Our Codes
        case Error::ALGORITHM_NOT_ALLOWED_INSTRUCTION:
            ret = "algorithm command on not allowed instruction";
            break;
        case Error::ALGORITHM_BAD_CONTAINER_WASNT_REJECT:
            ret = "invalid containers weren't rejected";
            break;
        case Error::ALGORITHM_IGNORED_CONTAINER_SHOULD_BE_LOADED:
            ret = "there are some valid containers in port that were ignored";
            break;
        case Error::ALGORITHM_IGNORED_CONTAINER_SHOULD_BE_UNLOADED:
            ret = "there are some containers with destination as the current port that that weren't unloaded";
            break;
        case Error::SHIP_ISNT_EMPTY_IN_END_OF_TRAVEL:
            ret = "the ship isn't empty in end of travel";
            break;
        case Error::ALGORITHM_FALSE_TRAVEL_ERROR:
            ret = "the algorithm reported travel error for vain";
            break;
        case Error::ALGORITHM_INVALID_COMMAND:
            ret = "algorithm provided invalid command to the crane management";
            break;
        case Error::ALGORITHM_INCORRECTLY_REJECTED_CONTAINER:
            ret = "a container was rejected due to lack of space, while another farther container was loaded";
            break;
        case Error::FAILED_TO_LOAD_ALGORITHM:
            ret = "failed to load some so files";
            break;
        default:
            ret = "";
    }

    if (tstms.size() > 0) {
        ret+= (". details:\n" + tstms);
    } else{
        ret += '\n';
    }
    ret += "================================================="
           "=========================================================";
    return ret;
}


Errors::Errors(int errors) : errors(errors) {}

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
    return errors & (int) error;
}

bool Errors::hasTravelError() {
    int fatalErrorCode = (int) Error::PLAN_FILE_CANNOT_BE_READ_ERROR | (int) Error::ROUTE_FILE_CANNOT_BE_READ_ERROR |
                         (int) Error::ROUTE_FILE_SINGLE_VALID_PORT_ERROR | (int) Error::DUPLICATE_X_Y_WITH_DIFFERENT_DATA;
    return errors & fatalErrorCode;;
}

void Errors::addErrors(Errors newErrors) {
    addErrors(newErrors.getErrorsCode());
}

void Errors::addErrors(int newErrors) {
    errors |= newErrors;
}

void Errors::addError(Error newError) {
    addErrors((int) newError);
}

void Errors::addError(Error newError, string testimony) {
    addError(newError);
    testimonies[newError].push_back(testimony);
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

Errors Errors::getTravelErrors() {
    int travelErrors = (int) Error::PLAN_FILE_CANNOT_BE_READ_ERROR & (int) Error::ROUTE_FILE_CANNOT_BE_READ_ERROR &
              (int) Error::ROUTE_FILE_SINGLE_VALID_PORT_ERROR & (int) Error::DUPLICATE_X_Y_WITH_DIFFERENT_DATA;
    return (Errors) (errors & travelErrors);
}

Errors::Errors() {}
