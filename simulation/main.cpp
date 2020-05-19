#include "Simulation.h"
#include "../common/utils/Registrar.h"

using std::list;


int main(int argc, char **argv) {

    string travel_path, algorithm_path, output_path;

    for(int i = 0 ; i < argc-1 ; i++)
    {
        if(std::string(argv[i]) == "-travel_path")
        {
            travel_path = argv[i+1];
        }
        if(std::string(argv[i]) == "-algorithm_path")
        {
            algorithm_path = argv[i+1];
        }
        if(std::string(argv[i]) == "-output")
        {
            output_path = argv[i+1];
        }
    }

    if(travel_path == "")
    {
        std::cout << "Missing travel path argument. program failed" << std::endl;
        return 1;
    }

    //TODO: add checks: valid path when is must be valid

    Simulation simulation(travel_path, algorithm_path, output_path);
    simulation.simulateAllTravelsWithAllAlgorithms();


//    Simulation simulation("C://input/travels", "", "C://input/output");
//    simulation.simulateAllTravelsWithAllAlgorithms();
    return 0;
}
