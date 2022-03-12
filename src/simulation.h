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

    void run(int timeSteps);

    int timeToEquilibrium();
    double autoCovariance(int tau);
    vector<double> autoCorrelations();

    void setTemperature(float T) { engine.setTemperature(T); };
    void setHField(float H) { engine.setHField(H); };
    void randomize(void) {lattice.randomize(); };
private:
    Engine engine;
    Lattice lattice;

    vector<double> magnetisations;
    vector<double> flips;

    int n;
};


#endif //ISINGMODEL_SIMULATION_H
