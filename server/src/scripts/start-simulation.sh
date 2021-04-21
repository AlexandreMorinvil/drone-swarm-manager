#!/bin/bash
sed -i -e "s/quantity=\"[0-9]*\"/quantity=\"$1\"/g" /root/simulation/experiments/sim-alfred.argos 
/root/simulation/launch_scripts/launch_simulation.sh