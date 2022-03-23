//
// Created by boris on 14/03/2022.
//

#ifndef ISINGMODEL_DECORRELATION_H
#define ISINGMODEL_DECORRELATION_H

#include "../src/simulation.h"

struct DecorResults
{
    int n;
    float T;
    bool randomised;
    std::vector<double> correlations;
    int decorrelationTime;
};

DecorResults getDecorrelationResults(Simulation& sim);

double autoCovariance(std::vector<double>& mag, int t_start, int tau);
std::vector<double> autoCorrelations(std::vector<double>& mag, int t_start);

optional<int> decorrelationTime(std::vector<double>& autoCorrelation);

void logDecorrelationResults(DecorResults& results);


#endif //ISINGMODEL_DECORRELATION_H
