//
// Created by boris on 10/03/2022.
//

#include <iostream>
#include "simulation.h"

using namespace std;

Simulation::Simulation(int n)
    :
    engine(),
    lattice(n),
    n(n)
{
   lattice.randomize();
   //engine.setTemperature(5);
   //engine.setHField(10);
}


void Simulation::runSimulation(int timeSteps)
{
    lattice.spins[0][1] = -1;
    lattice.spins[1][0] = -1;
    lattice.spins[2][1] = -1;
    lattice.spins[1][2] = -1;
    Lattice nextLattice = engine.timeStep(lattice);

    cout << lattice;

    printf("next step\n");

    cout << nextLattice;
}



void Simulation::timeToEquilibrium(int timeSteps)
{
    Lattice oldLattice(n);
    for (int i = 0; i < timeSteps; i++) {
        Lattice nextLattice = engine.timeStep(lattice);

        float eq = engine.measureLatticeChange(lattice, nextLattice);

        printf("eq = %f, mag = %f\n", eq, lattice.magnetisation());

        oldLattice = lattice;
        lattice = nextLattice;
    }

//    cout << oldLattice << endl;
//    cout << lattice;
}
