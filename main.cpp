#include <iostream>
#include "src\simulation.h"


int main() {
    std::cout << "Hello World" << std::endl;

    Simulation sim(1000);

    //sim.runSimulation(1);
    sim.setTemperature(0.1);

  //  sim.setHField(4);
    sim.timeToEquilibrium(15);

    std::cout << "switch\n";

    sim.setHField(0);
    sim.timeToEquilibrium(100);
}
