//
// Created by boris on 16/03/2022.
//

#ifndef ISINGMODEL_MEAN_MAGNETISATION_H
#define ISINGMODEL_MEAN_MAGNETISATION_H

#include <vector>
#include "../src/simulation.h"

struct MagResults
{
    int n;
    float T;
    bool randomised;
    int timeEquilibrium;
    double meanMagnetisation;
    std::vector<double> magnetisations;
};

MagResults getMagnetisationResults(Simulation& sim);
void logMagnetisationResults(MagResults& results);

#endif //ISINGMODEL_MEAN_MAGNETISATION_H
