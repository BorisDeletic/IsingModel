//
// Created by boris on 23/03/2022.
//

#include <set>
#include "run.h"
#include "spins_logger.h"
#include "zero_field.h"

void runSimulations()
{
   // vector<int> Ns = {50, 100, 200};
   // vector<float> Ts = {0.5, 1, 1.5, 2, 3};
   const int reps = 5;
   set<int> Ns = {10, 50, 100, 200};
   set<float> Ts;

    for (float T = 0; T < 2.2; T+=0.3) {
        Ts.insert(T);
    }
    for (float T = 2.2; T < 2.4; T += 0.025) {
        Ts.insert(T);
    }
    for (float T = 2.4; T < 4.2; T+=0.3) {
        Ts.insert(T);
    }

    for (int i = 0; i < reps; i++) {
        for (int n: Ns) {
            for (float T: Ts) {
                Simulation sim(n);
                ZeroField nofield;

                runZeroFieldSim(sim, T, initSteps, false);

                nofield.calculateResults(sim);
                nofield.logResults();
            }
        }
    }
}


void runSpinsWithCooling()
{
    int n = 500;
    int steps = 100;
    float Ti = 3;
    float Tf = 2.1;

    SpinLogger logger(n, steps * 3, Ti, Tf);

    Simulation sim(n);
    sim.setTemperature(Ti);

    cout << sim;

    for (int i = 0; i < steps; i++) {
        sim.run(1);
        logger.logSpins(sim);
    }

    sim.setTemperature(Tf);
    cout << sim;

    for (int i = 0; i < steps; i++) {
        sim.run(1);
        logger.logSpins(sim);
    }

    sim.setTemperature(1.0);
    cout << sim;

    for (int i = 0; i < steps * 2; i++) {
        sim.run(1);
        logger.logSpins(sim);
    }

    cout << sim;
}


void runZeroFieldSim(Simulation& sim, float T, int steps, bool randomised)
{
    if (randomised) sim.randomize();
    sim.setTemperature(T);

    int minAverageFactor = 10;
    int minSteps = maxSteps;
    optional<int> t_eq;
    optional<int> decorTime;

    int first_t_eq = -1;

    //while steps < minSteps
    while (sim.magnetisations.size() < minSteps || !t_eq || !decorTime)
    {
        sim.run(steps);
        t_eq = sim.timeToEquilibrium();

        if (t_eq) {

            vector<double> correlations = ZeroField::autoCorrelations(sim.magnetisations, *t_eq);
            decorTime = ZeroField::decorrelationTime(correlations);

            if (decorTime) {
                int longerEqTime = *decorTime > *t_eq ? *decorTime : *t_eq;
                minSteps = longerEqTime * minAverageFactor;
                minSteps = minSteps < maxSteps ? minSteps : maxSteps; // cap min steps at the maximum allowed
            }
        }

        steps *= 1.5;

        if (sim.magnetisations.size() > maxSteps) {
            printf("Equilibrium not reached in %d steps from main\n", sim.magnetisations.size());
            throw std::exception();
        }
    }


    cout << sim;
    cout << "T = " << T << endl;
    cout << "steps to decorrelate = " << *decorTime << endl;
    cout << "steps to eq = " << *t_eq << endl << endl;
}