#include <iostream>
#include <vector>
#include "src\simulation.h"

using namespace std;

int main()
{

}


void timeToEquilibrium()
{
    Simulation sim(1000);

    sim.setTemperature(3);
    sim.run(100);

    int time = sim.timeToEquilibrium();

    cout << "steps to eq = " << time << endl;
}


void autoCorrelation()
{
    Simulation sim(1000);

    sim.setTemperature(3);
    sim.run(100);

    vector<double> autoCors = sim.autoCorrelations();

    for (int i = 0; i < autoCors.size(); i++)
    {
        printf("autoCor %d = %f", i, autoCors[i]);
    }
}
