//
// Created by boris on 29/03/2022.
//

#include <algorithm>
#include <fstream>
#include "analysis.h"



Analysis::Analysis()
{
    ifstream f(fname);
    if (!f.good())
    {
        ofstream file;
        file.open(fname);

        file << "n, T, randomised, equilibriumTime, decorrelationTime, meanMagnetisation, spinVariance, heatCapacity, energyVariance" << endl;
        file << "magnetisations" << endl;
        file << "energys" << endl;
        file << "correlations" << endl;

        file.close();
    }
}


void Analysis::calculateResults(Simulation &sim) {
    float T = sim.getTemperature();

    optional<int> t_eq = sim.timeToEquilibrium();
    if (!t_eq) {
        // equilibrium conditions not reached
        printf("Equilibrium not reached in %d steps from absMags\n", sim.magnetisations.size());
        throw std::exception();
    }

    vector<double> correlations = autoCorrelations(sim.magnetisations, *t_eq);
    optional<int> decorTime = decorrelationTime(correlations);

    if (!decorTime) {
        printf("Did not decorrelate in %d steps\n", sim.magnetisations.size());
        throw std::exception();
    }

    // find mean of |M| instead of M
    vector<double> absMags;
    for (double mag : sim.magnetisations) {
        absMags.push_back(abs(mag));
    }


    results.n = sim.n;
    results.T = T;
    results.H = sim.getHField();
    results.randomised = sim.randomised ? 1 : 0;

    results.equilibriumTime = *t_eq;
    results.decorrelationTime = *decorTime;

    results.meanMagnetisation = reduce(absMags.begin() + *t_eq, absMags.end()) / (absMags.size() - *t_eq);
    results.spinVariance = sim.engine.fluctuations(sim.magnetisations, *t_eq);

    results.energyVariance = sim.engine.fluctuations(sim.energy, *t_eq);
    results.heatCapacity = T == 0 ? 0 : results.energyVariance / (pow(T, 2));

    results.magnetisations = sim.magnetisations;
    results.energys = sim.energy;
    results.correlations = correlations;
}



// DECORRELATIONS //
vector<double> Analysis::autoCorrelations(vector<double>& mags, int t_eq)
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


double Analysis::autoCovariance(vector<double>& mags, int t_start, int tau)
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
optional<int> Analysis::decorrelationTime(vector<double>& autoCorrelation)
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

//////////////////////////////////////////////////////////////////////////////


void Analysis::logResults()
{
    int rand = results.randomised ? 1 : 0;

    ofstream file;
    file.open(fname, fstream::app);

    file <<
        results.n << "," <<
        results.T << "," <<
        results.H << "," <<
        rand << "," <<
        results.equilibriumTime << "," <<
        results.decorrelationTime << "," <<
        results.meanMagnetisation << "," <<
        results.spinVariance << "," <<
        results.heatCapacity << "," <<
        results.energyVariance << "," << endl;


    for (double mag : results.magnetisations)
    {
        file << mag << ",";
    }
    file << endl;

    for (double energy : results.energys)
    {
        file << energy << ",";
    }
    file << endl;

    for (double correlation : results.correlations)
    {
        file << correlation << ",";
    }
    file << endl;

    file.close();
}

