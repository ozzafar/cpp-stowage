#include "Simulation.h"
#include "../common/utils/Registrar.h"
#include "../common/utils/IO.h"

using std::list;


int main(int argc, char **argv) {

    string travel_path, algorithm_path, output_path, num_threads_str;
    int num_threads = 1;

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
        if(std::string(argv[i]) == "-num_threads")
        {
            num_threads_str = argv[i+1];
        }

    }

    if(travel_path == "")
    {
        std::cout << "Missing travel path argument. program failed" << std::endl;
        return 1;
    }
    if(IO::isNumber(num_threads_str))
    {
        num_threads = std::stoi(num_threads_str);
    }

    Simulation simulation(travel_path, algorithm_path, output_path, num_threads);
    simulation.simulateAllTravelsWithAllAlgorithms();

    return 0;
}
