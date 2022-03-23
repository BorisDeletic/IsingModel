//
// Created by boris on 23/03/2022.
//

#include "run.h"
#include "../src/simulation.h"
#include "decorrelation.h"
#include "mean_magnetisation.h"

void runSimulations()
{
    vector<int> Ns = {50, 100};
    vector<float> Ts = {0, 0.5, 1, 2, 3};

    for (int n : Ns) {
        for (float T : Ts) {
            Simulation sim(n);

            runSim(sim, T, initSteps, false);

            MagResults magRes = getMagnetisationResults(sim);
            DecorResults decRes = getDecorrelationResults(sim);

            logMagnetisationResults(magRes);
            logDecorrelationResults(decRes);
        }
    }
}


void runSim(Simulation& sim, float T, int steps, bool randomised)
{
    if (randomised) sim.randomize();
    sim.setTemperature(T);

    optional<int> t_eq;
    optional<int> decorTime;
    vector<double> correlations;

    while (!t_eq && steps < maxSteps) {
        sim.run(steps);
        t_eq = sim.timeToEquilibrium();
        steps *= 2;
    }

// equilibrium conditions not reached
    if (!t_eq) {
        printf("Equilibrium not reached in %d steps\n", sim.magnetisations.size());
        throw std::exception();
    }

    correlations = autoCorrelations(sim.magnetisations, *t_eq);
    decorTime = decorrelationTime(correlations);

    while (!decorTime && steps < maxSteps) {
        sim.run(steps);

        correlations = autoCorrelations(sim.magnetisations, *t_eq);
        decorTime = decorrelationTime(correlations);

        steps *= 2;
    }

    // Did not decorrelate in time
    if (!decorTime) {
        printf("Did not decorrelate in %d steps\n", sim.magnetisations.size());
        throw std::exception();
    }

    cout << sim;
    cout << "T = " << T << endl;
    cout << "steps to eq = " << *t_eq << endl;
}