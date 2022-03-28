//
// Created by boris on 27/03/2022.
//

#ifndef ISINGMODEL_SPINS_H
#define ISINGMODEL_SPINS_H

#include <vector>
#include "../src/simulation.h"

class SpinLogger {
public:
    void logSpins(Simulation& sim);

    SpinLogger(int n, int steps, float T_init, float T_final);

private:
    string fname = R"(..\results\spins.csv)";
};


#endif //ISINGMODEL_SPINS_H
