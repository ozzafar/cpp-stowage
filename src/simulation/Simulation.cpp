//
// Created by Yarden on 03/05/2020.
//


#include "Simulation.h"
using namespace std;

Simulation::Simulation(const string &travelsPath, const string &algorithmPath, const string &outputPath): travelsPath(travelsPath), outputPath(outputPath)
{
    if(algorithmPath == "")
    {
        //load algorithms from cwd
    } else
    {
        //load algorithms from algorithmPath
    }

    NaiveAlgorithm *naiveAlgorithm1 = new NaiveAlgorithm(); //TODO: remove after testing and load from file
    NaiveAlgorithm *naiveAlgorithm2 = new NaiveAlgorithm();
    algorithms.push_back(naiveAlgorithm1);
    algorithms.push_back(naiveAlgorithm2);
}

int Simulation::simulateAllTravelsWithAllAlgorithms()
{
    if (travelsPath == "")
    {
        cout << "Fatal error: missing travel_path argument!" << endl;
        return 1;
    }

    std::ifstream path(travelsPath);
    if(path.good() == 0)
    {
        cout << "Fatal error: travel_path argument isn't valid!" << endl;
        return 1;
    }

    for(auto& travel: std::filesystem::directory_iterator(travelsPath))
    {
        string pathOfCurrentTravel = travel.path().string();
        simulateOneTravelWithAllAlgorithms(pathOfCurrentTravel);
    }

    return 0;
}

void Simulation::simulateOneTravelWithAllAlgorithms(const string &travelPath) {

    cout << "Started simulate " << travelPath << endl;

    const std::filesystem::path path = travelPath;

    if(is_directory(path) == 0) //travelPath isn't a directory
    {
        cout << "Travel error: " << travelPath << " is not a directory" << endl;
        return;
    }


    cout << "Finished simulate " << travelPath << endl;

}
