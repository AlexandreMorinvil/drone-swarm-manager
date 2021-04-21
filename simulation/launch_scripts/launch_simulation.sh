#!/bin/bash

# To launch the simulation
cd /root/simulation
cd ./experiments
g++ wallGen.cpp -o wallGen.out
./wallGen.out
cd /root/simulation/build
cmake ..
make
cd /root/simulation && argos3 -c experiments/sim-alfred.argos