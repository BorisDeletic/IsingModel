//
// Created by boris on 27/03/2022.
//

#ifndef ISINGMODEL_SPINS_H
#define ISINGMODEL_SPINS_H

#include <vector>

struct SpinResults
{
    int n;
    float T;
    bool randomised;
    int timeEquilibrium;
    std::vector<std::vector<int>> spins;
};


#endif //ISINGMODEL_SPINS_H
