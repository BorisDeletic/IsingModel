//
// Created by boris on 14/03/2022.
//

#ifndef ISINGMODEL_DECORRELATION_H
#define ISINGMODEL_DECORRELATION_H

#include <vector>

std::vector<double> autoCorrelations(std::vector<double>& mag, int t_start);

double autoCovariance(std::vector<double>& mag, int t_start, int tau);
int decorrelationTime(std::vector<double>& autoCorrelation);

void logResults(int n, float T, int steps, bool randomised = false);

void runDecorrelationSimulation();

#endif //ISINGMODEL_DECORRELATION_H
