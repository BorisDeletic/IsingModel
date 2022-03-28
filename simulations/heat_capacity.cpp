//
// Created by boris on 23/03/2022.
//

#include <fstream>
#include <algorithm>
#include <math.h>
#include "heat_capacity.h"


HeatResults getHeatCapacityResults(Simulation& sim)
{
    optional<int> t_eq = sim.timeToEquilibrium();
    if (!t_eq) {
        // equilibrium conditions not reached
        printf("Equilibrium not reached in %d steps from heat cap\n", sim.magnetisations.size());
        throw std::exception();
    }

    float T = sim.getTemperature();
    double energyStd = sim.engine.fluctuations(sim.energy, *t_eq);

    double heatCapacity = pow(energyStd, 2) / (pow(T, 2));
    heatCapacity = T == 0 ? 0 : heatCapacity;

    HeatResults res = {
            sim.n,
            T,
            sim.randomised,
            sim.energy,
            energyStd,
            heatCapacity
    };

    return res;
}



void logHeatCapacityResults(HeatResults& results)
{
    int rand = results.randomised ? 1 : 0;

    ofstream myfile;
    myfile.open(R"(..\results\heat_capacity.csv)", fstream::app);

    myfile << results.n << "," << results.T << "," << rand << "," << results.energyStd << "," << results.heatCapacity << endl;

    for (int i = 0; i < results.energy.size(); i++)
    {
        myfile << results.energy[i] << ",";
    }
    myfile << endl;
}
