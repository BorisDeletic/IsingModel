#include <iostream>
#include <vector>
#include "src\simulation.h"

using namespace std;



void timeToEquilibrium()
{
    Simulation sim(500);

    //sim.randomize();
    sim.setTemperature(4);
  //  sim.setHField(0.1);
    sim.run(600);

    int time = sim.timeToEquilibrium();

    cout << "steps to eq = " << time << endl;
}


void meanMag(float T)
{
    Simulation sim(500);

    sim.randomize();
    sim.setTemperature(T);
    sim.run(6000);

    vector<double> mags = sim.getMags();
    double meanMag = reduce(mags.begin(), mags.end()) / mags.size();

    cout << sim;
    cout << "T = " << T << endl;

    int time = sim.timeToEquilibrium();
    cout << "steps to eq = " << time << endl;

    cout << "mean mags = " << meanMag << endl << endl;


}


void autoCorrelation(float T)
{
    Simulation sim(100);
    sim.randomize();

    sim.setTemperature(T);
    sim.run(500);

    vector<double> autoCors = sim.autoCorrelations();

 //   int t_eq = sim.timeToEquilibrium();

    for (int i = 0; i < autoCors.size(); i++)
    {
        printf("autoCor %d = %f\n", i, autoCors[i]);
    }
}


int main()
{
 //   meanMag(30);
//    meanMag(15);
 //   meanMag(5);
    meanMag(0.5);

   // timeToEquilibrium();

   // autoCorrelation(1);

   // autoCorrelation(3);

  //  autoCorrelation(2);

  //  autoCorrelation(1.5);
}
