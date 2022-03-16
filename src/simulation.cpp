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
    n(n)
{
}


void Simulation::run(int timeSteps)
{
    for (int i = 0; i < timeSteps; i++) {
        Lattice nextLattice = engine.timeStep(lattice);

        float eq = engine.fractionSpinsFlipped(lattice, nextLattice);

        flips.push_back(eq);
        magnetisations.push_back(lattice.magnetisation());

      //  printf("%d: eq = %f, mag = %f\n", i, eq, lattice.magnetisation());

        lattice = nextLattice;
    }
}


int Simulation::timeToEquilibriumF() {
/*
 * Calculate the number of steps before magnetisations stabilises -> equilibrium
 * We consider magnetisations stabilised when they does not fluctuate more than 1% from mean
 * For window steps.
 */
    const int windowSize = 20;
    const float fluctuationThreshold = 0.01;

    for (int i = 0; i < flips.size() - windowSize; i++) {
        auto start = flips.begin() + i;
        auto end = flips.begin() + i + windowSize;

        float meanFlips = reduce(start, end) / windowSize;
        float maxFlips  = *max_element(start, end);
        float minFlips  = *min_element(start, end);

        if (
                (maxFlips - meanFlips) / meanFlips < fluctuationThreshold &&
                (meanFlips - minFlips) / meanFlips < fluctuationThreshold)
        {
            return i;
        }
    }

    // equilibrium conditions not reached
    printf("Equilibrium not reached");
    throw std::exception();

}


int Simulation::timeToEquilibriumM() {
/*
 * Calculate the number of steps before magnetisations stabilises -> equilibrium
 * We consider magnetisations stabilised when they does not fluctuate more than 1% from mean
 * For window steps.
 */
    const int windowSize = 50;
    const float slopeThreshold = 0.0001;

     for (int i = 0; i < magnetisations.size() - windowSize; i++) {
         auto start = magnetisations.begin() + i;
         auto end = magnetisations.begin() + i + windowSize;

        vector<double> magsWindow;
        for (auto it = start; it < end; it++)
        {
            magsWindow.push_back(*it / (n*n)); //fractional magnetisation
        }

        double slope = gradientLineBestFit(magsWindow);
        cout << "slope = " << slope << endl;

        if (abs(slope) < slopeThreshold) {
            return i;
        }
    }

    // equilibrium conditions not reached
    printf("Equilibrium not reached");
    throw std::exception();

}


double gradientLineBestFit(const std::vector<double> &pts) {
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
    if( std::fabs(denominator) < 1e-7 ) {
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
