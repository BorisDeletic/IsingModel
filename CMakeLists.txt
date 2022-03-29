cmake_minimum_required(VERSION 3.21)
project(IsingModel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

include_directories(simulations)

add_executable(IsingModel
        main.cpp
        src/lattice.cpp
        src/lattice.h
        src/engine.cpp
        src/engine.h
        src/simulation.cpp
        src/simulation.h
        simulations/run.cpp
        simulations/run.h
        simulations/spins_logger.cpp
        simulations/spins_logger.h
        simulations/zero_field.cpp
        simulations/zero_field.h)

