//
// Created by boris on 10/03/2022.
//

#include <iostream>
#include <algorithm>
#include "simulation.h"

using namespace std;

Simulation::Simulation(int n)
    :
    engine(),
    lattice(n),
    n(n)
{
}




void Simulation::run(int timeSteps)
{
    for (int i = 0; i < timeSteps; i++) {
        Lattice nextLattice = engine.timeStep(lattice);

        float eq = engine.fractionSpinsFlipped(lattice, nextLattice);

        flips.push_back(eq);
        magnetisations.push_back(lattice.magnetisation());

     //   printf("eq = %f, mag = %f\n", eq, lattice.magnetisation());

        lattice = nextLattice;
    }
}


int Simulation::timeToEquilibrium() {
/*
 * Calculate the number of steps before flips stabilises -> equilibrium
 * We consider flips stabilised when they does not fluctuate more than 1% from mean
 * For window steps.
 */
    const int windowSize = 20;
    const float fluctuationThreshold = 0.01;

    for (int i = 0; i < flips.size() - windowSize; i++) {
        auto start = flips.begin() + i;
        auto end = flips.begin() + i + windowSize;

        float meanFlips = reduce(start, end);
        float maxFlips  = *max_element(start, end);
        float minFlips  = *min_element(start, end);

        if (
                (maxFlips - meanFlips) / meanFlips < fluctuationThreshold &&
                (meanFlips - minFlips) / meanFlips < fluctuationThreshold)
        {
            return i;
        }
    }

    // equilibrium conditions not reached
    return -1;

}



double Simulation::autoCovariance(int tau)
{
    double meanMagnetisation = reduce(magnetisations.begin(), magnetisations.end()) / magnetisations.size();

    int t = magnetisations.size() - tau;
    if (t / magnetisations.size() < 0.9)
    {
        std::cout << "Not averaging over long enough time\n";
        return 0;
    }

    vector<double> covariance(t);

    for (int i = 0; i < t; i++)
    {
        double magP = magnetisations[i] - meanMagnetisation;
        double magP_tau = magnetisations[i + tau] - meanMagnetisation;

        covariance[i] = magP * magP_tau;
    }

    double autoCov = reduce(covariance.begin(), covariance.end()) / t;

    return autoCov;
}


vector<double> Simulation::autoCorrelations()
{
    double autoCov0 = autoCovariance(0);

    vector<double> autoCor;

    for (int tau = 0; tau < magnetisations.size(); tau++)
    {
        autoCor.push_back(autoCovariance(tau) / autoCov0);
    }

    return autoCor;
}

