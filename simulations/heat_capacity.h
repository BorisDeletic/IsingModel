//
// Created by boris on 23/03/2022.
//

#ifndef ISINGMODEL_HEAT_CAPACITY_H
#define ISINGMODEL_HEAT_CAPACITY_H

#include <vector>

struct HeatResults
{
    int n;
    float T;
    bool randomised;
    std::vector<double> correlations;
    int decorrelationTime;
};


#endif //ISINGMODEL_HEAT_CAPACITY_H
