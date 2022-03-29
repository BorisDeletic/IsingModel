//
// Created by Boris Deletic on 12/03/2022.
//

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <optional>
#include "decorrelation.h"



// Local function declarations
///////////////////////////////////////////////////////////////////////////////////



using namespace std;



DecorResults getDecorrelationResults(Simulation& sim)
{

    optional<int> t_eq = sim.timeToEquilibrium();
    if (!t_eq) {
        // equilibrium conditions not reached
        printf("Equilibrium not reached in %d steps from decor\n", sim.magnetisations.size());
        throw std::exception();
    }

    vector<double> correlations = autoCorrelations(sim.magnetisations, *t_eq);
    optional<int> decorTime = decorrelationTime(correlations);

    if (!decorTime) {
        printf("Did not decorrelate in %d steps\n", sim.magnetisations.size());
        //throw std::exception();
        decorTime = 0;
    }


    DecorResults res = {
            sim.n,
            sim.getTemperature(),
            sim.randomised,
            correlations,
            *decorTime
    };

    return res;
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

        cor = autoCov0 == 0 ? 0 : cor;
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


//   Decorrelation time is how long before auto correlation falls to 1/e
optional<int> decorrelationTime(vector<double>& autoCorrelation)
{
    double decayValue = 0.367879441;

    for (int i = 0; i < autoCorrelation.size(); i++)
    {
        if (autoCorrelation[i] < decayValue) {
            return i;
        }
    }

    return nullopt;
}


void logDecorrelationResults(DecorResults& results)
{
    int rand = results.randomised ? 1 : 0;

    ofstream myfile;
    myfile.open(R"(..\results\decorrelation_times.csv)", fstream::app);

    myfile << results.n << "," << results.T << "," << rand << "," << results.decorrelationTime << endl;

    for (int i = 0; i < results.correlations.size(); i++)
    {
        myfile << results.correlations[i] << ",";
    }
    myfile << endl;

    myfile.close();
}

