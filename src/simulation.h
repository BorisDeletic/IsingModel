//
// Created by boris on 10/03/2022.
//

#ifndef ISINGMODEL_SIMULATION_H
#define ISINGMODEL_SIMULATION_H

#include "engine.h"
#include "lattice.h"

class Simulation {
public:
    Simulation(int n);

    void runSimulation(int timeSteps);
    void timeToEquilibrium(int timeSteps);

    void setTemperature(float T) { engine.setTemperature(T); };
    void setHField(float H) { engine.setHField(H); };
private:
    Engine engine;
    Lattice lattice;

    int n;
};


#endif //ISINGMODEL_SIMULATION_H
