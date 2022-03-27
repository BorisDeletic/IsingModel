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
        simulations/decorrelation.cpp
        simulations/decorrelation.h simulations/mean_magnetisation.cpp simulations/mean_magnetisation.h simulations/run.cpp simulations/run.h simulations/heat_capacity.cpp simulations/heat_capacity.h simulations/spins.cpp simulations/spins.h)

