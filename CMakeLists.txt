cmake_minimum_required(VERSION 3.15)
project(cpp_stowage)

set(CMAKE_CXX_STANDARD 14)

add_executable(cpp_stowage main.cpp common/Container.cpp common/Container.h common/Ship.cpp common/Ship.h common/ShipPlan.cpp common/ShipPlan.h common/ContainersPosition.cpp common/ContainersPosition.h common/WeightBalanceCalculator.cpp common/WeightBalanceCalculator.h common/Algorithm.cpp common/Algorithm.h common/Simulation.cpp common/Simulation.h common/Route.cpp common/Route.h common/Travel.cpp common/Travel.h)

add_subdirectory(tests)