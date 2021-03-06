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


void Engine::timeStep(Lattice& lattice)
{
    if (monteCarlo) {
        calculateNewSpinsMC(lattice.spins);
    } else {
        calculateNewSpinsLA(lattice.spins);
    }
}


//calculate linearly going through lattice
void Engine::calculateNewSpinsLA(vector<vector<signed char>> &newSpins)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            signed char s0 = newSpins[i][j];

            // get neighbour spins and use wrap-around boundary conditions
            signed char s1 = newSpins[i - 1 >= 0 ? i - 1 : n - 1][j];
            signed char s2 = newSpins[i + 1 < n ? i + 1 : 0][j];
            signed char s3 = newSpins[i][j - 1 >= 0 ? j - 1 : n - 1];
            signed char s4 = newSpins[i][j + 1 < n ? j + 1 : 0];

            newSpins[i][j] = flipSpin(s0, {s1, s2, s3, s4});
        }
    }
}

// pick spins to flip randomly
void Engine::calculateNewSpinsMC(vector<vector<signed char>> &newSpins)
{

    for (int t = 0; t < n * n; t++) {
        int i = spinRNG(gen);
        int j = spinRNG(gen);

        signed char s0 = newSpins[i][j];

        // get neighbour spins and use wrap-around boundary conditions
        signed char s1 = newSpins[i - 1 >= 0 ? i - 1 : n - 1][j];
        signed char s2 = newSpins[i + 1 < n ? i + 1 : 0][j];
        signed char s3 = newSpins[i][j - 1 >= 0 ? j - 1 : n - 1];
        signed char s4 = newSpins[i][j + 1 < n ? j + 1 : 0];

        newSpins[i][j] = flipSpin(s0, {s1, s2, s3, s4});
    }
}


signed char Engine::flipSpin(signed char s0, vector<signed char> neighbours) {
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


double Engine::fluctuations(vector<double>& quantity_raw, int t0)
{
    vector<double> quantity;
    for (double q : quantity_raw) {
        quantity.push_back(fabs(q));
    }

    const int len = quantity.size() - t0;

    double mean = reduce(quantity.begin() + t0, quantity.end()) / len;

    vector<double> rmsQuantity;

    for (int i = t0; i < quantity.size(); i++) {
        double rms = pow(quantity[i] - mean, 2);
        rmsQuantity.push_back(rms);
    }

    double variance = reduce(rmsQuantity.begin(), rmsQuantity.end()) / rmsQuantity.size();

    return variance;
}




#pragma clang diagnostic pop