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



float Lattice::magnetisation() {
    float mag = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            mag += spins[i][j];
        }
    }

    return mag / (n * n);
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
