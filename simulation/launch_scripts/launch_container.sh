#!/bin/bash

BASEDIR=$(dirname "$0")
SIMULATION_CONTAINER_ID=$(docker ps -qf "name=simulation-argos")

# Launch the container if it was not launched
if [ -z $SIMULATION_CONTAINER_ID ]
then
    docker build -f $BASEDIR/../Dockerfile $BASEDIR/.. --tag simulation-argos --network host
    docker rmi $(docker images --filter "dangling=true" -q --no-trunc)
    x11docker --hostdisplay --hostnet --user=RETAIN -- --privileged simulation-argos &
    sleep 10
    SIMULATION_CONTAINER_ID=$(docker ps -qf "name=simulation-argos")
fi

# Enter the container
echo "ARGoS Launch Script : You will now enter the container."
docker exec -it $SIMULATION_CONTAINER_ID /bin/bash

# This script will execute once we have left the container
docker stop $SIMULATION_CONTAINER_ID
echo "ARGoS Launch Script : The simulation container was stoped" 
echo "ARGoS Launch Script : You are now outside the conatiner. The simulation container was stoped" 