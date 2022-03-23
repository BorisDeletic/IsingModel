//
// Created by boris on 23/03/2022.
//

#ifndef ISINGMODEL_RUN_H
#define ISINGMODEL_RUN_H


#include "../src/simulation.h"

void runSimulations();

void runSim(Simulation& sim, float T, int steps, bool randomised);

const int maxSteps = 5000;
const int initSteps = 100;


#endif //ISINGMODEL_RUN_H
