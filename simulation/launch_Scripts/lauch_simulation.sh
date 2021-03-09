#!/bin/bash

# To enter the container
# docker exec -it $(docker ps -qf "name=simulation-argos") /bin/bash 

# To launch the simulation
cd /root/simulation
cd ./experiments
./wallGen.out
cd /root/simulation/cmake
cmake ..
make
cd /root/simulation && argos3 -c experiments/demo_pdr.argos"
