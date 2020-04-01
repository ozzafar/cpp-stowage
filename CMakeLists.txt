cmake_minimum_required(VERSION 3.15)
project(cpp_stowage)

set(CMAKE_CXX_STANDARD 14)

add_executable(cpp_stowage main.cpp common/Container.cpp common/Container.h)