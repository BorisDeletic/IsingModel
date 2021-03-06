//
// Created by boris on 23/03/2022.
//

#include <set>
#include "run.h"
#include "spins_logger.h"
#include "analysis.h"

void runZeroFieldSimulations()
{
   const int reps = 10;
   set<int> Ns = {10, 20, 30, 50, 100, 200};
   set<float> Ts;

    for (float T = 0; T < 5; T+=0.3) {
        Ts.insert(T);
    }
    for (float T = 2.25; T < 3.3; T+=0.05) {
        Ts.insert(T);
    }

// for large n
  //  Ts.insert(2.2692);
  //  Ts.insert(2.2695);
   // Ts.insert(2.271);
  //  Ts.insert(2.269);
//    for (float T = 2.269; T < 2.274; T += 0.001) {
//        Ts.insert(T);
//    }
//    for (float T = 2.0; T < 3.101; T += 0.1) {
//        Ts.insert(T);
//    }

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

    int decorTimeFactor = 10;
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
                minSteps = *decorTime * decorTimeFactor;
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

            logger.logSpins(sim);
        }

        cout << sim;
    }
}
