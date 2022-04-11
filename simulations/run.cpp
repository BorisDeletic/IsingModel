//
// Created by boris on 23/03/2022.
//

#include <set>
#include "run.h"
#include "spins_logger.h"
#include "analysis.h"

void runZeroFieldSimulations()
{
   // vector<int> Ns = {50, 100, 200};
   // vector<float> Ts = {0.5, 1, 1.5, 2, 3};
   const int reps = 5;
   set<int> Ns = {30, 50, 100, 200, 500};
   set<float> Ts;

    for (float T = 0; T < 2.2; T+=0.3) {
        Ts.insert(T);
    }
    for (float T = 2.2; T < 2.4; T += 0.025) {
        Ts.insert(T);
    }
    for (float T = 2.4; T < 5; T+=0.3) {
        Ts.insert(T);
    }
    Ts.insert(2.269);
    Ts.insert(2.271);
    Ts.insert(2.270);
    Ts.insert(2.272);
    Ts.insert(2.273);
    Ts.insert(2.274);
    Ts.insert(2.276);
    Ts.insert(2.277);
    Ts.insert(2.278);
    Ts.insert(2.279);

    for (int i = 0; i < reps; i++) {
        for (int n: Ns) {
            for (float T: Ts) {
                Simulation sim(n);
                Analysis nofield;

                runSim(sim, T, initSteps, false);

                nofield.calculateResults(sim);
                nofield.logResults();
            }
        }
    }
}


void runFieldSimulations(float H)
{
    // vector<int> Ns = {50, 100, 200};
    // vector<float> Ts = {0.5, 1, 1.5, 2, 3};
    const int reps = 5;
    int n = 100;
    set<float> Ts;

    for (float T = 0; T < 10; T+=0.3) {
        Ts.insert(T);
    }

    for (int i = 0; i < reps; i++) {
        for (float T: Ts) {
            Simulation sim(n);
            sim.setHField(H);

            Analysis nonzerofield;
            nonzerofield.fname = R"(..\results\H_field.csv)";

            runSim(sim, T, initSteps, false);

            nonzerofield.calculateResults(sim);
            nonzerofield.logResults();

        }
    }
}





void runSim(Simulation& sim, float T, int steps, bool randomised)
{
    if (randomised) sim.randomize();
    sim.setTemperature(T);

    int minAverageFactor = 10;
    int minSteps = maxSteps;
    optional<int> t_eq;
    optional<int> decorTime;

    //while steps < minSteps
    while (sim.magnetisations.size() < minSteps || !t_eq || !decorTime)
    {
        sim.run(steps);
        t_eq = sim.timeToEquilibrium();

        if (t_eq) {

            vector<double> correlations = Analysis::autoCorrelations(sim.magnetisations, *t_eq);
            decorTime = Analysis::decorrelationTime(correlations);

            if (decorTime) {
                int longerEqTime = *decorTime > *t_eq ? *decorTime : *t_eq;
                minSteps = longerEqTime * minAverageFactor;
                minSteps = minSteps < maxSteps ? minSteps : maxSteps; // cap min steps at the maximum allowed
            }
        }

        steps *= 1.5;

        if (sim.magnetisations.size() > maxSteps) {
            printf("Equilibrium not reached in %d steps from main\n", sim.magnetisations.size());
            t_eq = 10000;
         //   throw std::exception();
        }
    }


    cout << sim;
    cout << "T = " << T << endl;
    cout << "H = " << sim.getHField() << endl;
    cout << "steps to decorrelate = " << *decorTime << endl;
    cout << "steps to eq = " << *t_eq << endl << endl;
}



void runSpinsWithCooling()
{
    int n = 100;
    int steps = 100;
    vector<float> Ts = {3, 2.1, 1.0};

    SpinLogger logger(n, steps * Ts.size());

    Simulation sim(n);
    cout << sim;


    for (float T : Ts) {
        sim.setTemperature(T);
        for (int i = 0; i < steps; i++) {
            sim.run(1);

            logger.logState(sim);
            logger.logSpins(sim);
        }
    }
}


void runSpinsWithHField(float T)
{
    int n = 500;
    int steps = 2;
    vector<float> Hs;
    for (float H = -0.5; H < 0.5; H+=0.01) {
        Hs.push_back(H);
    }
    for (float H = 0.5; H > -0.5; H-=0.01) {
        Hs.push_back(H);
    }

    SpinLogger logger(n, steps * Hs.size());

    Simulation sim(n);
    sim.randomize();
    sim.setTemperature(T);
    cout << sim;

    for (float H : Hs) {
        sim.setHField(H);
        for (int i = 0; i < steps; i++) {
            sim.run(1);
            logger.logState(sim);
            logger.logSpins(sim);
        }

        cout << sim;
    }
}
