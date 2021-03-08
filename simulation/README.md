# INF3995
> **Warning:** You need to associate an `ed25519` SSH key with your gitlab account in order to continue.
## Run simulation using shell commands

1. Clone this repository :          $`git clone git@gitlab.com:polytechnique-montr-al/inf3995/20211/equipe-204/ALFRED.git`
2. Go to your `.ssh` folder with    $`cd ~/.ssh`
3. Build image with                 $`docker build -f ~/path/to/repo/simulation/Dockerfile . --tag simulation-argos --network host`
4. Run x11docker client with        $`x11docker --hostdisplay --hostnet --user=RETAIN -- --privileged simulation-argos`
5. Retreive container ID with       $`docker ps`
6. Open the container with          $`docker exec -it *ID_FOUND_WITH_DOCKER_PS* /bin/bash`
7. Generate random walls (optional) $` cd ~/alfred/simulation/experiments && ./randomWalls.out`
8. Start simulation with            $`$ cd ~/alfred/simulation && argos3 -c experiments/demo_pdr.argos`


## Launch using shell scripts (will generate random walls each time): 
1. run launchX11.sh with x11docker : `$ source launch_scripts/launchX11.sh`
2. open bash in the containe (see steps 5-6 of running using shell commands)
3. start simulation with randomly generated walls : `$ ~/alfred/simulation/launch_scripts && source launch_simulation.sh`