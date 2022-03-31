//
// Created by Boris Deletic on 28/03/2022.
//

#include <fstream>
#include "hysteresis.h"
#include "../src/simulation.h"



void runHysteresisLoop(int n, float T, vector<float>& Hs)
{
    ofstream file;
    file.open(R"(..\results\hysteresis.csv)", fstream::app);

    file << n << "," << T << endl;

    int initSteps = 500;
    int steps = 100;


    Simulation sim(n);
    sim.setTemperature(T);
    sim.setHField(Hs[0]);

    sim.run(initSteps);

    for (int i = 0; i < Hs.size(); i++)
    {
        float H = Hs[i];
        sim.setHField(H);
        sim.run(steps);

        int start = sim.magnetisations.size() - steps;

        vector<double> mags;
        for (int i = start; i < sim.magnetisations.size(); i++) {
            mags.push_back(sim.magnetisations[i]);
        }

        double meanMag = reduce(mags.begin(), mags.end()) / mags.size();

        file << H << "," << meanMag << endl;

        cout << sim;
        cout << "T = " << sim.getTemperature() << endl;
        cout << "H = " << sim.getHField() << endl << endl;

    }

    file << endl;
    file.close();

}