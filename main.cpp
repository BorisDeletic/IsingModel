#include <iostream>
#include <vector>
#include "src/simulation.h"
#include "simulations/decorrelation.h"

using namespace std;



void timeToEquilibrium()
{
    Simulation sim(100);

    //sim.randomize();
    sim.setTemperature(4);
  //  sim.setHField(0.1);
    sim.run(100);

    int time = sim.timeToEquilibriumM();

    cout << "steps to eq = " << time << endl;
}


void meanMag(float T)
{
    Simulation sim(100);

    sim.randomize();
    sim.setTemperature(T);
    sim.run(1000);

    vector<double> mags = sim.magnetisations;
    double meanMag = reduce(mags.begin(), mags.end()) / mags.size();

    cout << sim;
    cout << "T = " << T << endl;

    int time = sim.timeToEquilibriumM();
    cout << "steps to eq = " << time << endl;

    cout << "mean mags = " << meanMag << endl << endl;


}



int main()
{
 //   meanMag(30);
//    meanMag(15);
 //   meanMag(5);
   // meanMag(2);
    //logResults(200, 3, 500, true);
    logResults(200, 6, 500, false);
   // timeToEquilibrium();

   // autoCorrelation(1);

   // autoCorrelation(3);

  //  autoCorrelation(2);

  //  autoCorrelation(1.5);
}
