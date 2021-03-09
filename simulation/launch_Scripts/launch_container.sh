#!/bin/bash
docker build -f ../Dockerfile . --tag simulation-argos --network host
x11docker --hostdisplay --hostnet --user=RETAIN -- --privileged simulation-argos &
docker exec -it $(docker ps -qf "name=simulation-argos") /bin/bash

echo "YOU ARE NOW INSIDE THE ARGOS SIMULATION CONTAINER"