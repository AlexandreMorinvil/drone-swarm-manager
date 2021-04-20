#!/bin/bash

SIMULATION_CONTAINER_ID=$(docker ps -qf "name=simulation-argos")

# Launch the container if it was  launched
if [ -n $SIMULATION_CONTAINER_ID ]
then
    # This script will execute once we have left the container
    docker stop $SIMULATION_CONTAINER_ID
    echo "ARGoS Launch Script : The simulation container was stoped" 
fi