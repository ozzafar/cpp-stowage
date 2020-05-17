cmake_minimum_required(VERSION 3.15)
project(cpp_stowage)

set(CMAKE_CXX_STANDARD 20)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++2a -Wall -Wextra -Werror -pedantic-errors -DNDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(cpp_stowage interfaces/AbstractAlgorithm.h common/objects/Container.cpp common/objects/Container.h common/objects/Ship.cpp common/objects/Ship.h common/objects/ShipPlan.cpp common/objects/ShipPlan.h common/objects/ContainersPosition.cpp common/objects/ContainersPosition.h algorithm/WeightBalanceCalculator.cpp algorithm/WeightBalanceCalculator.h algorithm/NaiveAlgorithm.cpp algorithm/NaiveAlgorithm.h simulation/AlgorithmResults.h simulation/AlgorithmResults.cpp simulation/Simulation.cpp simulation/Simulation.h common/objects/Route.cpp common/objects/Route.h common/objects/Travel.cpp common/objects/Travel.h common/objects/CraneManagement.cpp common/objects/CraneManagement.h common/utils/IO.cpp common/utils/IO.h
        common/utils/Errors.h common/utils/Errors.cpp algorithm/BasicAlgorithm.cpp algorithm/BasicAlgorithm.h simulation/main.cpp interfaces/AlgorithmRegistration.h common/utils/Registrar.cpp common/utils/Registrar.h algorithm/_206039984_a.h interfaces/AlgorithmRegistration.cpp algorithm/_206039984_a.cpp algorithm/NaiveAlgorithmHorizontal.cpp algorithm/NaiveAlgorithmHorizontal.h algorithm/NaiveAlgorithmVertical.cpp algorithm/NaiveAlgorithmVertical.h common/utils/ErrorsIterator.cpp common/utils/ErrorsIterator.h common/utils/Error.h algorithm/_206039984_b.cpp algorithm/_206039984_b.h algorithm/_206039984_c.cpp algorithm/_206039984_c.h)

add_subdirectory(tests)