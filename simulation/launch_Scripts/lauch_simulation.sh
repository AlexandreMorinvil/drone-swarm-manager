#!/bin/bash
cd experiments
./wallGen.out
cd ../cmake
cmake ..
make
cd ~/alfred/simulation && argos3 -c experiments/demo_pdr.argos