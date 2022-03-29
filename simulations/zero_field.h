//
// Created by boris on 29/03/2022.
//

#ifndef ISINGMODEL_ZERO_FIELD_H
#define ISINGMODEL_ZERO_FIELD_H

#include <vector>
#include <optional>
#include "../src/simulation.h"

using namespace std;

struct Results
{
    int n;
    float T;
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


class ZeroField {
public:
    ZeroField();

    void calculateResults(Simulation& sim);


    // DECORRELATIONS //
    static optional<int> decorrelationTime(vector<double>& autoCorrelation);
    static vector<double> autoCorrelations(vector<double>& mags, int t_eq);
    static double autoCovariance(vector<double>& mags, int t_start, int tau);


    void logResults();

private:
    Results results;
    string fname = R"(..\results\zero_field.csv)";
};




#endif //ISINGMODEL_ZERO_FIELD_H
