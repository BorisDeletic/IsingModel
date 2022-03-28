#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"
//
// Created by boris on 10/03/2022.
//

#include <math.h>
#include <algorithm>
#include "engine.h"
#include "lattice.h"


Engine::Engine(int n)
    :
    n(n),
    T(0),
    H(0),
    gen(rd()),
    tempRNG(0.0, 1.0), //setup random number generator
    spinRNG(0, n-1)
{
}


Lattice Engine::timeStep(Lattice& old)
{
    Lattice newLattice(n);
    newLattice.spins = old.spins;

    if (monteCarlo) {
        calculateNewSpinsMC(newLattice.spins);
    } else {
        calculateNewSpinsLA(newLattice.spins);
    }

    return newLattice;
}


//calculate linearly going through lattice
void Engine::calculateNewSpinsLA(vector<vector<int>> &newSpins)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int s0 = newSpins[i][j];

            // get neighbour spins and use wrap-around boundary conditions
            int s1 = newSpins[i - 1 >= 0 ? i - 1 : n - 1][j];
            int s2 = newSpins[i + 1 < n ? i + 1 : 0][j];
            int s3 = newSpins[i][j - 1 >= 0 ? j - 1 : n - 1];
            int s4 = newSpins[i][j + 1 < n ? j + 1 : 0];

            newSpins[i][j] = flipSpin(s0, {s1, s2, s3, s4});
        }
    }
}

void Engine::calculateNewSpinsMC(vector<vector<int>> &newSpins)
{

    for (int t = 0; t < n * n; t++) {
        int i = spinRNG(gen);
        int j = spinRNG(gen);

        int s0 = newSpins[i][j];

        // get neighbour spins and use wrap-around boundary conditions
        int s1 = newSpins[i - 1 >= 0 ? i - 1 : n - 1][j];
        int s2 = newSpins[i + 1 < n ? i + 1 : 0][j];
        int s3 = newSpins[i][j - 1 >= 0 ? j - 1 : n - 1];
        int s4 = newSpins[i][j + 1 < n ? j + 1 : 0];

        newSpins[i][j] = flipSpin(s0, {s1, s2, s3, s4});
    }
}


int Engine::flipSpin(int s0, vector<int> neighbours) {
    int sFlipped = s0 == 1 ? -1 : 1;

    float neighbourTerm = 0.0f;
    float neighbourTermFlipped = 0.0f;
    for (int i = 0; i < 4; i++)
    {
        neighbourTerm += s0 * neighbours[i];
        neighbourTermFlipped += sFlipped * neighbours[i];
    }

    float Enow = - neighbourTerm - H * s0;
    float Eflip = - neighbourTermFlipped - H * sFlipped;

    float deltaE = Eflip - Enow;

    if (deltaE < 0.0f) { // flip the spin
        return sFlipped;
    }

    double thermalEnergy = exp(- deltaE / T);
    double threshold = tempRNG(gen);

    if (thermalEnergy > threshold)
    {
        return sFlipped;
    }

    return s0;
}


float Engine::fractionSpinsFlipped(Lattice &oldLattice, Lattice &newLattice) {
    float spinsFlipped = 0;
    int n = oldLattice.spins.size();

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (oldLattice.spins[i][j] != newLattice.spins[i][j])
            {
                spinsFlipped++;
            }
        }
    }

    return spinsFlipped / (n*n);
}



double fluctuations(vector<double>& quantity, int t0)
{
    const int len = quantity.size() - t0;

    double meanEnergy = reduce(quantity.begin() + t0, quantity.end()) / len;

    vector<double> rmsQuantity;

    for (int i = t0; i < quantity.size(); i++) {
        double rms = pow(quantity[i] - meanEnergy, 2);
        rmsQuantity.push_back(rms);
    }

    double meanRMS = reduce(rmsQuantity.begin(), rmsQuantity.end()) / rmsQuantity.size();
    double std = sqrt(meanRMS);

    return std;
}




#pragma clang diagnostic pop