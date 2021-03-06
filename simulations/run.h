//
// Created by boris on 23/03/2022.
//

#ifndef ISINGMODEL_RUN_H
#define ISINGMODEL_RUN_H


#include "../src/simulation.h"

void runZeroFieldSimulations();
void runFieldSimulations(float H);
void runSpinsWithCooling();
void runSpinsWithHField(float T);

void runSim(Simulation& sim, float T, int steps, bool randomised);

const int maxSteps = 200000;
const int initSteps = 5000;


#endif //ISINGMODEL_RUN_H
