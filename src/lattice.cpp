#include "lattice.h"
#include <stdlib.h>


Lattice::Lattice(int n)
    :
    n(n),
    spins(vector<vector<int>>(n, vector<int>(n, 1)))
{

}


void Lattice::randomize() {
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int spin = rand() % 2 == 0 ? 1 : -1;
            spins[i][j] = spin;
        }
    }
}



double Lattice::magnetisation() {
    double mag = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            mag += spins[i][j];
        }
    }

    return mag;
}


double Lattice::energy(float H) {
    double energy = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int s0 = spins[i][j];

            // get neighbour spins and use wrap-around boundary conditions
            int s1 = spins[i - 1 >= 0 ? i - 1 : n - 1][j];
            int s2 = spins[i + 1 < n ? i + 1 : 0][j];
            int s3 = spins[i][j - 1 >= 0 ? j - 1 : n - 1];
            int s4 = spins[i][j + 1 < n ? j + 1 : 0];

            double neighbours = (s0 * s1) + (s0 * s2) + (s0 * s3) + (s0 * s4);
            double Hterm = H * s0;

            energy += -neighbours - Hterm;
        }
    }

    return energy;
}



ostream &operator<<(ostream &o, const Lattice &l) {
    for (int i = 0; i < l.spins.size(); i++) {
        for (int j = 0; j < l.spins.size(); j++) {
            o << l.spins[i][j] << "\t";
        }
        o << endl;
    }

    return o;
}

