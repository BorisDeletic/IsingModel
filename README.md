<h1>Ising Model</h1>

This project is a computational exploration into simulating a ferromagnet using the Ising model and measuring its physical observables with statistical methods. The program can simulate the evolution of a spin lattice and calculate macroscopic properties such as magnetisation, susceptibility, heat capacity and others. The critical phase transition temperature was measured up to a lattice of size \(N = 400\) with a value \(T_c(N=400) = 2.2690 \pm 0.0015 \) which is in agreement with analytically derived values for the Ising model.

The Ising Model is a simple statistical model which aims to represent a ferromagnetic material. The model considers a 2 dimensional lattice of spins pointing either up or down with simple dynamics governing how the spins evolve.	

![Image](https://raw.githubusercontent.com/BorisDeletic/IsingModel/main/report/resources/ising_model.PNG)

The model uses the [Metropolis algorithm](https://en.wikipedia.org/wiki/Metropolis-Hastings_algorithm) to calculate the dynamics of the spin lattice in time. 

The full report can be viewed in the PDF.
