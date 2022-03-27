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
    double energyStd = energyFluctuations(sim.energy, *t_eq);

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


double energyFluctuations(vector<double>& energy, int t_eq)
{
    const int len = energy.size() - t_eq;

    double meanEnergy = reduce(energy.begin() + t_eq, energy.end()) / len;

    vector<double> rmsEnergy;

    for (int i = t_eq; i < energy.size(); i++) {
        double rms = pow(energy[i] - meanEnergy, 2);
        rmsEnergy.push_back(rms);
    }

    double meanRMS = reduce(rmsEnergy.begin(), rmsEnergy.end()) / rmsEnergy.size();
    double std = sqrt(meanRMS);

    return std;
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
