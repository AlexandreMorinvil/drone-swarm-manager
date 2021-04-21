#!/bin/bash

BASEDIR=$(dirname "$0")
SIMULATION_CONTAINER_ID=$(docker ps -qf "name=simulation-argos")

# Launch the container if it was not launched
if [ -z $SIMULATION_CONTAINER_ID ]
then
    docker build -f $BASEDIR/../Dockerfile $BASEDIR/.. --tag simulation-argos --network host
    docker rmi $(docker images --filter "dangling=true" -q --no-trunc)
    x11docker --hostdisplay --hostnet --user=RETAIN -- --privileged simulation-argos &
fi