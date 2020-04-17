cmake_minimum_required(VERSION 3.15)
project(cpp_stowage)

set(CMAKE_CXX_STANDARD 20)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(cpp_stowage main.cpp common/Container.cpp common/Container.h common/Ship.cpp common/Ship.h common/ShipPlan.cpp common/ShipPlan.h common/ContainersPosition.cpp common/ContainersPosition.h common/WeightBalanceCalculator.cpp common/WeightBalanceCalculator.h common/NaiveAlgorithm.cpp common/NaiveAlgorithm.h common/Simulation.cpp common/Simulation.h common/Route.cpp common/Route.h common/Travel.cpp common/Travel.h common/CraneManagement.cpp common/CraneManagement.h common/CraneOperation.h common/Algorithm.cpp common/Algorithm.h)

add_subdirectory(tests)