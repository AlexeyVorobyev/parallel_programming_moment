cmake_minimum_required(VERSION 3.28)
project(parallel)

set(CMAKE_CXX_STANDARD 17)

find_package(MPI REQUIRED)

# Enable OpenMP support
# Link MPI libraries

#add_executable(parallel main.cpp
#        lab3/main.cpp
#        lab3/mainPrallel.cpp)

#add_executable(parallel lab3/main.cpp)
add_executable(parallel lab3/mainParallel.cpp)
