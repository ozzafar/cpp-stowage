cmake_minimum_required(VERSION 3.15)
project(cpp_stowage)

set(CMAKE_CXX_STANDARD 20)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(cpp_stowage main.cpp src/Container.cpp src/Container.h src/Ship.cpp src/Ship.h src/ShipPlan.cpp src/ShipPlan.h src/ContainersPosition.cpp src/ContainersPosition.h src/WeightBalanceCalculator.cpp src/WeightBalanceCalculator.h src/NaiveAlgorithm.cpp src/NaiveAlgorithm.h src/Simulation.cpp src/Simulation.h src/Route.cpp src/Route.h src/Travel.cpp src/Travel.h src/CraneManagement.cpp src/CraneManagement.h src/CraneOperation.h src/Algorithm.cpp src/Algorithm.h common/IO.cpp common/IO.h)

add_subdirectory(tests)