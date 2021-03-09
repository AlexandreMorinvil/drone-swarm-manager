#!/bin/bash

# To enter the container
docker exec -it $(docker ps -qf "name=simulation-argos") /bin/bash

# To launch the simulation
cd experiments
./wallGen.out
cd ../cmake
cmake ..
make
cd ~/alfred/simulation && argos3 -c experiments/demo_pdr.argos