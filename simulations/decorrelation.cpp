//
// Created by Boris Deletic on 12/03/2022.
//

#include <vector>
#include <algorithm>
#include <iostream>
#include "../src/simulation.h"

using namespace std;

double autoCovariance(vector<double>& mag, int t_start, int tau);
vector<double> autoCorrelations(vector<double>& mag);
int decorrelationTime(vector<double>& autoCorrelation);


void logResults(int n, float T, int steps)
{
    Simulation sim(n);
    sim.setTemperature(T);
    sim.run(steps);

    vector<double> correlations = autoCorrelations(sim.magnetisations);
    int decorTime = decorrelationTime(correlations);


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


vector<double> autoCorrelations(vector<double>& mags)
{
    float correlationCutoff = 0.2; 
    // the maximum offset we can calculate correlation for is 20% of total time
    
    //  int t_eq = timeToEquilibrium();
    int t_eq = 0;
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
