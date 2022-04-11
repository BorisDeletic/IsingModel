
#include <fstream>
#include <chrono>
#include "simulations/hysteresis.h"
#include "simulations/run.h"
#include "analysis.h"

using namespace std;


void measureSpeed();
void hysteresis();

int main()
{

    runZeroFieldSimulations();

    //runSpinsWithCooling();
   // runSpinsWithHField(3.0);

//    vector<float> Hs = {0.1, 0.2, 0.4, 0.6, 0.8, 1.0, 1.5, 2.0};
//    for (float H : Hs) {
//        runFieldSimulations(H);
//    }
    measureSpeed();
}


void hysteresis()
{
    vector<float> Hs;
    vector<float> Hs2;
    for (float H = -0.25; H <= 0.25; H+=0.005) {
        Hs.push_back(H);
    }

    for (float H = -0.5; H <= 0.5; H+=0.01) {
        Hs2.push_back(H);
    }

    runHysteresisLoop(100, 2.1, Hs);
    runHysteresisLoop(100, 2.2, Hs);
    runHysteresisLoop(100, 2.3, Hs);
    runHysteresisLoop(100, 2.6, Hs);
    runHysteresisLoop(100, 1.5, Hs2);

}

void measureSpeed()
{
    ofstream file;
    file.open(R"(..\results\speed.csv)");

    // should take approx 12h
    vector<int> Ns = {10, 20, 50, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    int steps = 10000;


    for (int montecarlo = 0; montecarlo < 2; montecarlo++) {
        for (int n : Ns) {
            auto t1 = std::chrono::high_resolution_clock::now();

            Simulation sim(n);
            sim.engine.monteCarlo = !(montecarlo == 0);
            sim.run(steps);

            auto t2 = std::chrono::high_resolution_clock::now();

            /* Getting number of milliseconds as an integer. */
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

            file << n << "," << montecarlo << "," << steps << "," << time.count() << endl;

            cout << sim << endl;
        }
    }

    file.close();
}
