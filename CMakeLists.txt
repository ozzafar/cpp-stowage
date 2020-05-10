cmake_minimum_required(VERSION 3.15)
project(cpp_stowage)

set(CMAKE_CXX_STANDARD 20)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(cpp_stowage interfaces/AbstractAlgorithm.h common/objects/Container.cpp common/objects/Container.h common/objects/Ship.cpp common/objects/Ship.h common/objects/ShipPlan.cpp common/objects/ShipPlan.h common/objects/ContainersPosition.cpp common/objects/ContainersPosition.h algorithm/WeightBalanceCalculator.cpp algorithm/WeightBalanceCalculator.h algorithm/NaiveAlgorithm.cpp algorithm/NaiveAlgorithm.h simulation/Simulation.cpp simulation/Simulation.h common/objects/Route.cpp common/objects/Route.h common/objects/Travel.cpp common/objects/Travel.h common/objects/CraneManagement.cpp common/objects/CraneManagement.h common/utils/IO.cpp common/utils/IO.h
        common/utils/Errors.h common/utils/Errors.cpp algorithm/BasicAlgorithm.cpp algorithm/BasicAlgorithm.h simulation/main.cpp interfaces/AlgorithmRegistration.h common/utils/Registrar.cpp common/utils/Registrar.h algorithm/_206039984_a.h interfaces/AlgorithmRegistration.cpp algorithm/_206039984_a.cpp)

add_subdirectory(tests)