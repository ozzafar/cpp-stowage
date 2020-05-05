cmake_minimum_required(VERSION 3.15)
project(cpp_stowage)

set(CMAKE_CXX_STANDARD 20)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(cpp_stowage main.cpp common/objects/AbstractAlgorithm.h common/objects/Container.cpp common/objects/Container.h common/objects/Ship.cpp common/objects/Ship.h common/objects/ShipPlan.cpp common/objects/ShipPlan.h common/objects/ContainersPosition.cpp common/objects/ContainersPosition.h src/algorithm/WeightBalanceCalculator.cpp src/algorithm/WeightBalanceCalculator.h src/algorithm/NaiveAlgorithm.cpp src/algorithm/NaiveAlgorithm.h src/simulation/Simulation.cpp src/simulation/Simulation.h common/objects/Route.cpp common/objects/Route.h common/objects/Travel.cpp common/objects/Travel.h src/algorithm/CraneManagement.cpp src/algorithm/CraneManagement.h src/algorithm/AbstractAlgorithm.h common/utils/IO.cpp common/utils/IO.h
        common/errors/Errors.h common/errors/Errors.cpp src/algorithm/BasicAlgorithm.cpp src/algorithm/BasicAlgorithm.h)

add_subdirectory(tests)