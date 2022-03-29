//
// Created by boris on 16/03/2022.
//

#include <optional>
#include <fstream>
#include "mean_magnetisation.h"
#include "../src/simulation.h"




MagResults getMagnetisationResults(Simulation& sim)
{
    optional<int> t_eq = sim.timeToEquilibrium();
    if (!t_eq) {
        // equilibrium conditions not reached
        printf("Equilibrium not reached in %d steps from mags\n", sim.magnetisations.size());
        throw std::exception();
    }


    // find mean of |M| instead of M
    vector<double> mags;
    for (double mag : sim.magnetisations) {
        mags.push_back(abs(mag));
    }

    double meanMag = reduce(mags.begin() + *t_eq, mags.end()) / (mags.size() - *t_eq);
    double spinFluctuations = sim.engine.fluctuations(sim.magnetisations, *t_eq);

    MagResults res = {
            sim.n,
            sim.getTemperature(),
            sim.randomised,
            *t_eq,
            meanMag,
            spinFluctuations,
            sim.magnetisations
    };

    return res;
}


void logMagnetisationResults(MagResults& results)
{
    int rand = results.randomised ? 1 : 0;

    ofstream myfile;
    myfile.open(R"(..\results\mean_magnetisations.csv)", fstream::app);

    myfile
        << results.n << ","
        << results.T << ","
        << rand << ","
        << results.timeEquilibrium << ","
        << results.meanMagnetisation << ","
        << results.spinFluctuation << endl;

    for (double mag : results.magnetisations)
    {
        myfile << mag << ",";
    }

    myfile << endl;
    myfile.close();
}
