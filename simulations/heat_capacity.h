//
// Created by boris on 23/03/2022.
//

#ifndef ISINGMODEL_HEAT_CAPACITY_H
#define ISINGMODEL_HEAT_CAPACITY_H

#include <vector>
#include "../src/simulation.h"

struct HeatResults
{
    int n;
    float T;
    bool randomised;
    std::vector<double> energy;
    double energyStd;
    double heatCapacity;
};

HeatResults getHeatCapacityResults(Simulation& sim);
double energyFluctuations(vector<double>& energy, int t_eq);
void logHeatCapacityResults(HeatResults& results);

#endif //ISINGMODEL_HEAT_CAPACITY_H
