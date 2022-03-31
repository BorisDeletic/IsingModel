//
// Created by boris on 29/03/2022.
//

#ifndef ISINGMODEL_ANALYSIS_H
#define ISINGMODEL_ANALYSIS_H

#include <vector>
#include <optional>
#include "../src/simulation.h"

using namespace std;

struct Results
{
    int n;
    float T;
    float H = 0.0;
    bool randomised;

    int equilibriumTime;
    int decorrelationTime;

    double meanMagnetisation;
    double spinVariance;

    double heatCapacity;
    double energyVariance;

    vector<double> magnetisations;
    vector<double> energys;
    vector<double> correlations;
};


class Analysis {
public:
    Analysis();

    void calculateResults(Simulation& sim);


    // DECORRELATIONS //
    static optional<int> decorrelationTime(vector<double>& autoCorrelation);
    static vector<double> autoCorrelations(vector<double>& mags, int t_eq);
    static double autoCovariance(vector<double>& mags, int t_start, int tau);


    void logResults();

    string fname = R"(..\results\zero_field.csv)";
private:
    Results results;
};




#endif //ISINGMODEL_ANALYSIS_H
