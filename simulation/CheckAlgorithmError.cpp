////
//// Created by Oz Zafar on 20/05/2020.
////
//
//#include "CheckAlgorithmError.h"
//
//std::string CheckAlgorithmError::errorToString(CheckAlgorithmError::Error error) {
//    switch (error) {
//        case Error::LOADED_PORT_DESTINATION_IS_CURRENT_PORT:
//            return "can't load container with destination port as the current port";
//        case Error::CONTAINER_DESTINATION_ISNT_IN_NEXT_STOPS:
//            return "can't load container with destination port which isn't in the route's next stops";
//        case Error::REJECTED_NOT_FAR_CONTAINERS:
//            return "not invalid container was rejected though another one with farther destination was loaded";
//        case Error::BAD_CONTAINER_WASNT_REJECTED:
//            return "invalid container was loaded";
//        case Error::CONTAINER_WASNT_REVIEWED:
//            return "there are some valid containers in port that were ignored";
//        case Error::SHIP_ISNT_EMPTY_IN_END_OF_TRAVEL:
//            return "the ship isn't empty in end of travel";
//        case Error::ALGORITHM_ROUTE_ERROR:
//            return "the algorithm failed to read correct route file";
//        case Error::ALGORITHM_SHIP_PLAN_ERROR:
//            return "the algorithm failed to read correct ship plan file";
//        case Error::ALGORITHM_INCORRECT_TRAVEL_ERROR_REPORT:
//            return "the algorithm reported travel error for vain";
//        case Error::ALGORITHM_INVALID_COMMAND:
//            return "algorithm provided invalid command to the crane management";
//        case Error::FULL_CONTAINER_POSITION:
//            return "can't load container to a full position";
//        case Error::UNLOAD_NOT_TOP_CONTAINER:
//            return "can't load container to a full position";
//        default:
//            return "";
//    }
//}
