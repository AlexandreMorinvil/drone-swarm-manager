#!/bin/bash
docker exec $(docker ps -qf "name=simulation-argos") sed -i -e "s/quantity=\"[0-9]*\"/quantity=\"$1\"/g" /root/simulation/experiments/sim-alfred.argos 
docker exec $(docker ps -qf "name=simulation-argos") /root/simulation/launch_scripts/launch_simulation.sh
