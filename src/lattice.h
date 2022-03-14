//
// Created by boris on 07/03/2022.
//

#ifndef ISINGMODEL_LATTICE_H
#define ISINGMODEL_LATTICE_H

#include <vector>
#include <iostream>

using namespace std;

class Lattice {
public:
    Lattice(int n);

    void randomize();
    double magnetisation();
    double energy(float H);

    vector<vector<int>> spins;

    friend ostream& operator << (ostream &o, const Lattice &l);
private:
    int n;
};



#endif //ISINGMODEL_LATTICE_H
