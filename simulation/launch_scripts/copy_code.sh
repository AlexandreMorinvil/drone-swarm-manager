#!/bin/bash

# To launch the simulation
docker cp $(docker ps -qf "name=simulation-argos"):/root/simulation/controllers/ ../
docker cp $(docker ps -qf "name=simulation-argos"):/root/simulation/experiments/ ../
