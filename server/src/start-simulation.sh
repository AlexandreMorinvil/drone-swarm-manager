#!/bin/bash
docker exec $(docker ps -qf "name=simulation-argos") /root/simulation/launch_scripts/launch_simulation.sh
