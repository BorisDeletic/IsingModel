#pragma clang diagnostic push
#pragma ide diagnostic ignored "openmp-use-default-none"
//
// Created by boris on 10/03/2022.
//

#include <math.h>
#include <algorithm>
#include "engine.h"
#include "lattice.h"


Engine::Engine()
    :
    T(0),
    H(0),
    gen(rd()),
    rng(0.0, 1.0) //setup random number generator
{
}


Lattice Engine::timeStep(Lattice& old)
{
    Lattice newLattice(old.spins.size());

    calculateNewSpins(newLattice.spins, old.spins);

    return newLattice;
}


void Engine::calculateNewSpins(vector<vector<int>> &newSpins, vector<vector<int>> &oldSpins)
{
    int n = oldSpins.size();

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int s0 = oldSpins[i][j];

            // get neighbour spins and use wrap-around boundary conditions
            int s1 = oldSpins[i - 1 >= 0 ? i - 1 : n - 1][j];
            int s2 = oldSpins[i + 1 < n ? i + 1 : 0][j];
            int s3 = oldSpins[i][j - 1 >= 0 ? j - 1 : n - 1];
            int s4 = oldSpins[i][j + 1 < n ? j + 1 : 0];

            newSpins[i][j] = flipSpin(s0, {s1, s2, s3, s4});
        }
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

    float thermalEnergy = exp(- deltaE / T);
    float threshold = rng(gen);

    if (thermalEnergy > threshold)
    {
        return sFlipped;
    }

    return s0;
}


float Engine::measureLatticeChange(Lattice &oldLattice, Lattice &newLattice) {
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






#pragma clang diagnostic pop