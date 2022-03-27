//
// Created by boris on 23/03/2022.
//

#include <set>
#include "run.h"
#include "heat_capacity.h"
#include "decorrelation.h"
#include "mean_magnetisation.h"

void runSimulations()
{
   // vector<int> Ns = {50, 100, 200};
   // vector<float> Ts = {0.5, 1, 1.5, 2, 3};
   const int reps = 10;
   set<int> Ns = {100};
   set<float> Ts;

    for (float T = 0; T <= 3.5; T+=0.3) {
        Ts.insert(T);
    }
    for (float T = 2.0; T < 2.5; T+=0.05) {
        Ts.insert(T);
    }
    for (float T = 2.2; T <= 2.4; T += 0.025) {
        Ts.insert(T);
    }

    for (int i = 0; i < reps; i++) {
        for (int n: Ns) {
            for (float T: Ts) {
                Simulation sim(n);

                runSim(sim, T, initSteps, false);

                MagResults magRes = getMagnetisationResults(sim);
                DecorResults decRes = getDecorrelationResults(sim);
                HeatResults heatRes = getHeatCapacityResults(sim);

                logMagnetisationResults(magRes);
                logDecorrelationResults(decRes);
                logHeatCapacityResults(heatRes);
            }
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

    while ((!t_eq || !decorTime) && steps < maxSteps) {
        sim.run(steps);
        t_eq = sim.timeToEquilibrium();

        if (t_eq) {
            correlations = autoCorrelations(sim.magnetisations, *t_eq);
            decorTime = decorrelationTime(correlations);
        }

        steps *= 1.5;
    }

// equilibrium conditions not reached
    if (!t_eq) {
        printf("Equilibrium not reached in %d steps from main\n", sim.magnetisations.size());
        throw std::exception();
    }

    // Did not decorrelate in time
    if (!decorTime) {
        printf("Did not decorrelate in %d steps main\n", sim.magnetisations.size());
        throw std::exception();
    }

    cout << sim;
    cout << "T = " << T << endl;
    cout << "steps to eq = " << *t_eq << endl;
}