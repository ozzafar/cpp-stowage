//
// Created by Oz Zafar on 18/04/2020.
//

#ifndef CPP_STOWAGE_IO_H
#define CPP_STOWAGE_IO_H


#include "../objects/Container.h"
#include "../objects/Ship.h"
#include "../objects/Route.h"
#include "../../simulator/AlgorithmResults.h"
#include "Errors.h"
#include <vector>
#include <stdio.h>
#include <ctype.h>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem>

using std::vector;

class IO {

public:

    // push containers into waitingContainers param
    static int readContainerAwaitingAtPortFile(const string &input_path, Ship& ship, vector<Container>& waitingContainers,vector<Container>& badContainers, Errors& errors);

    // update plan param with the read plan
    static int readShipPlan(const string &path, ShipPlan& plan,Errors& errors);

    // update route param with the read route
    static int readShipRoute(const string &path,Route& route,Errors& errors);

    static vector<string> breakLineToWords(string &line, char delimeter);

    static int writeToFile(const string &writingPath, const string &content);

    static void writeResultsOfSimulation(const string &resultOutputPath, const vector<string> &travelNames, map<string, AlgorithmResults> &algorithmsResults);

    static void writeErrorsOfTravelAndAlgorithm(Errors &simErrors,Errors &algErrors, const string &outputPathOfErrorsFile);

    static bool isNumber(const std::string& s);

    static string firstFileWithExtensionInDirectory(const string& pathOfDirectory, const string &extension);

    static void clearPreviousOutput(const string& outputPath);

    static void ltrim(std::string &s);

    static void rtrim(std::string &s);

    static void trim(std::string &s);

    static int createPositionFromRowInput(int numOfFloors, int X, int Y, string &line, ShipPlan& shipPlan,Errors& errors);
    };

#endif //CPP_STOWAGE_IO_H
