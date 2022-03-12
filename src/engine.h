//
// Created by boris on 10/03/2022.
//

#ifndef ISINGMODEL_ENGINE_H
#define ISINGMODEL_ENGINE_H

#include <random>
#include "lattice.h"

class Engine {
public:
    Engine();

    Lattice timeStep(Lattice& old);

    void setTemperature(float temperature) {T = temperature; };
    void setHField(float field) {H = field; };

    float fractionSpinsFlipped(Lattice& oldLattice, Lattice& newLattice);
private:
    void calculateNewSpins(vector<vector<int>>& newSpins, vector<vector<int>>& oldSpins);
    int flipSpin(int s0, vector<int> neighbours);

    float T;
    float H;

    random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen; // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> rng;
};


#endif //ISINGMODEL_ENGINE_H
