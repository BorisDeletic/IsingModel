//
// Created by boris on 27/03/2022.
//

#include "spins_logger.h"
#include <fstream>


SpinLogger::SpinLogger(int n, int steps, float Ti, float Tf)
{
    ofstream myfile;
    myfile.open(fname);

    myfile << n << "," << steps << "," << Ti << "," << Tf << endl;
    myfile.close();
}


void SpinLogger::logSpins(Simulation &sim) {
    int n = sim.lattice.spins.size();
    ofstream myfile;
    myfile.open(fname, fstream::app);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            myfile << sim.lattice.spins[i][j] << ",";
        }
    }
    myfile << endl;

    myfile.close();
}
