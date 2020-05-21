//
// Created by Oz Zafar on 15/05/2020.
//

#ifndef CPP_STOWAGE_ERROR_H
#define CPP_STOWAGE_ERROR_H

enum class Error {

    SUCCESS                                        = 0,

    // Ship Plan Codes
    NUMBER_OF_FLOORS_WARNING                       = 1 << 0,
    ILLEGAL_POSITION_WARNING                       = 1 << 1,
    PLAN_BAD_LINE_FORMAT_WARNING                   = 1 << 2,
    PLAN_FILE_CANNOT_BE_READ_ERROR                 = 1 << 3,
    DUPLICATE_X_Y_WITH_DIFFERENT_DATA              = 1 << 4,

    // Reserved Codes
    RESERVED2                                      = 1 << 9,

    // Route Codes
    PORT_APPEAR_TWICE_WARNING                      = 1 << 5,
    BAD_PORT_SYMBOL_WARNING                        = 1 << 6,
    ROUTE_FILE_CANNOT_BE_READ_ERROR                = 1 << 7,
    ROUTE_FILE_SINGLE_VALID_PORT_ERROR             = 1 << 8,

    // Container Codes
    DUPLICATE_CONTAINER_ID_WARINING                = 1 << 10,
    CONTAINER_ID_ALREADY_IN_SHIP_WARINING          = 1 << 11,
    MISSING_OR_BAD_WEIGHT_WARINING                 = 1 << 12,
    MISSING_OR_BAD_DEST_WARINING                   = 1 << 13,
    ID_CANNOT_BE_READ_WARINING                     = 1 << 14,
    ILLEGAL_ID_CHECK_WARNING                       = 1 << 15,
    CONTAINER_FILE_CANNOT_BE_READ_WARNING          = 1 << 16,
    LAST_PORT_HAS_CONTAINERS_WARNING               = 1 << 17,
    PASS_TOTAL_CONTAINERS_AMOUNT_LIMIT_WARNING     = 1 << 18,

    // Crane Management Extra Codes
    // as long as the algorithm itself doesn't reports these - all good
    ALGORITHM_INVALID_COMMAND                      = 1 << 19,
    FULL_CONTAINER_POSITION                        = 1 << 20,
    UNLOAD_NOT_TOP_CONTAINER                       = 1 << 21
};


#endif //CPP_STOWAGE_ERROR_H
