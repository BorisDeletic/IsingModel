//
// Created by boris on 27/03/2022.
//

#include "spins_logger.h"
#include <fstream>


SpinLogger::SpinLogger(int n, int steps)
{
    ofstream myfile;
    myfile.open(fname);

    myfile << n << "," << steps << endl;
    myfile.close();
}




void SpinLogger::logSpins(Simulation &sim) {
    int n = sim.lattice.spins.size();
    float T = sim.getTemperature();
    float H = sim.getHField();
    double mag = sim.magnetisations[sim.magnetisations.size() - 1];

    ofstream myfile;
    myfile.open(fname, fstream::app);

    myfile << T << "," << H << "," << mag << endl;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            myfile << (int)sim.lattice.spins[i][j] << ",";
        }
    }
    myfile << endl;

    myfile.close();
}
