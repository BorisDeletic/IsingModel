//
// Created by Boris Deletic on 12/03/2022.
//

#include <vector>
#include <algorithm>
#include <iostream>
#include "decorrelation.h"
#include "../src/simulation.h"

using namespace std;


void runDecorrelationSimulation()
{
    vector<int> Ns = {10, 100, 1000, 10000};
    vector<float> Ts = {0, 0.5, 1, 2, 4, 8, 16};


    for (int n : Ns) {
        for (int i = 0; i < Ts.size(); i++) {
            logResults(n, Ts[i], 1000);
        }
    }

}


void logResults(int n, float T, int steps, bool randomised)
{
    Simulation sim(n);

    if (randomised) sim.randomize();
    sim.setTemperature(T);

    sim.run(steps);

    cout << sim;
    cout << "T = " << T << endl;
    int time = sim.timeToEquilibriumM();
    cout << "steps to eq = " << time << endl;

    vector<double> correlations = autoCorrelations(sim.magnetisations, time);
    int decorTime = decorrelationTime(correlations);



    char fname[100];
    sprintf(fname, "..\\results\\decorrelation_n%d_T%.1f.csv", n, T);

    freopen(fname, "w", stdout);
    freopen("..\\results\\decorrelation_times.csv", "a", stderr);

    for (int i = 0; i < correlations.size(); i++)
    {
        cout << correlations[i] << ",";
    }

    cerr << n << "," << T << "," << decorTime << endl;
}



//   Decorrelation time is how long before auto correlation falls to 1/e
int decorrelationTime(vector<double>& autoCorrelation)
{
    double decayValue = 0.367879441;

    for (int i = 0; i < autoCorrelation.size(); i++)
    {
        if (autoCorrelation[i] < decayValue) {
            return i;
        }
    }

    cout << "Does not decorrelate in simulation, run for longer" << endl;
    throw std::exception();
}


vector<double> autoCorrelations(vector<double>& mags, int t_eq)
{
    float correlationCutoff = 0.2; 
    // the maximum offset we can calculate correlation for is 20% of total time
    
    double autoCov0 = autoCovariance(mags, t_eq, 0);

    vector<double> autoCors;

    float maxTau = (mags.size() - t_eq) * correlationCutoff;

    for (int tau = 0; tau < maxTau; tau++)
    {
        double cor = autoCovariance(mags, t_eq, tau) / autoCov0;
        autoCors.push_back(cor);
    }

    return autoCors;
}


double autoCovariance(vector<double>& mags, int t_start, int tau)
{
    double meanMagnetisation = reduce(mags.begin() + t_start, mags.end()) / (mags.size() - t_start);

    vector<double> covariance;

    for (int i = t_start; i < mags.size() - tau; i++)
    {
        double magP = mags[i] - meanMagnetisation;
        double magP_tau = mags[i + tau] - meanMagnetisation;

        covariance.push_back(magP * magP_tau);
    }

    double meanCov = reduce(covariance.begin(), covariance.end()) / covariance.size();

    return meanCov;
}
