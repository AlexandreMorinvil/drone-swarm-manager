# INF3995 ARGOS simulation

## Run simulation using shell commands

1. Build image with                 $`docker build -f Dockerfile . --tag simulation-argos --network host`
2. Run x11docker client with        $`x11docker --hostdisplay --hostnet --user=RETAIN -- --privileged simulation-argos`
4. Open the container with          $`docker exec -it $(docker ps -qf "name=simulation-argos") /bin/bash`
5. Generate random walls (optional) $` cd /root/simulation/experiments && ./randomWalls.out`
6. Start simulation with            $`$ cd /root/simulation && argos3 -c experiments/sim-alfred.argos`


## Launch using shell scripts (will generate random walls each time): 
1. Run launchX11.sh with x11docker : `$ source launch_scripts/launchX11.sh`
2. Open bash in the containe (see steps 5-6 of running using shell commands)
3. Start simulation with randomly generated walls : `$ /root/simulation/launch_scripts && source launch_simulation.sh`

## Run all simulation tests (in container):
1. In container:  $` cd /root/simulation/build`
2. Run all tests:  $` ./launch_all_tests.sh`