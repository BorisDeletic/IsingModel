//
// Created by boris on 10/03/2022.
//

#include <iostream>
#include <algorithm>
#include "simulation.h"

using namespace std;

double gradientLineBestFit(const std::vector<double> &pts);

Simulation::Simulation(int n)
    :
    engine(n),
    lattice(n),
    n(n),
    magnetisations(),
    flips()
{
}


void Simulation::run(int timeSteps)
{
    for (int i = 0; i < timeSteps; i++) {
        magnetisations.push_back(lattice.magnetisation());
        energy.push_back(lattice.energy(getHField()));

        engine.timeStep(lattice);
    }
}


optional<int> Simulation::timeToEquilibrium() {
/*
 * Calculate the number of steps before energy stabilises -> equilibrium
 * We consider energy stabilised when the line of best fit is flat.
 * Only looking at the energy in a window (1/10th of the total time).
 */
  //  return energy.size()/10;
    const int steps = energy.size();

    const int windowSize = steps / 10;
    const float slopeThreshold = 0.0005;

     for (int i = windowSize; i < steps - windowSize; i+=5) {
         auto start = energy.begin() + i;
         auto end = energy.begin() + i + windowSize;

        vector<double> window;
        for (auto it = start; it < end; it++)
        {
            window.push_back(*it / (n*n)); //fractional magnetisation
        }

        double slope = gradientLineBestFit(window);

        if (abs(slope) < slopeThreshold) {
            return i;
        }
    }

     if (steps > 90000) {
         return 10000;
     }
    // equilibrium conditions not reached
    return nullopt;
}


double gradientLineBestFit(const vector<double> &pts)
{
    int nPoints = pts.size();
    if( nPoints < 2 ) {
        return 0;
    }
    double sumX=0, sumY=0, sumXY=0, sumX2=0;
    for(int i=0; i<nPoints; i++) {
        sumX += i;
        sumY += pts[i];
        sumXY += i * pts[i];
        sumX2 += i * i;
    }
    double xMean = sumX / nPoints;
    double yMean = sumY / nPoints;
    double denominator = sumX2 - sumX * xMean;
    // You can tune the eps (1e-7) below for your specific task
    if( fabs(denominator) < 1e-7 ) {
        // Fail: it seems a vertical line
        return 1e7;
    }
    double slope = (sumXY - sumX * yMean) / denominator;

    return slope;
}




ostream &operator<<(ostream &o, const Simulation &s) {
    o << "Simulation Parameters:" << endl;
    o << "n = " << s.n << endl;
    o << "steps = " << s.magnetisations.size() << endl;

    return o;
}
