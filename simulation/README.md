# INF3995

## Run simulation with Argos
**Warning:** You need to associate a SSH key with your gitlab account in order to continue.
1. Clone this repository : `git clone git@gitlab.com:polytechnique-montr-al/inf3995/20211/equipe-204/ground-station.git`
2. Go to your `.ssh` folder with `cd ~/.shh`
3. Build image with `docker build -f ~/path/to/repo/simulation/Dockerfile . --tag simulation-argos --network host`
4. Run x11docker client with `x11docker --hostdisplay --hostnet --user=RETAIN -- --privileged simulation-argos`
5. Retreive container ID with `docker ps`
6. Open the container with `docker exec -it *ID_FOUND_WITH_DOCKER_PS* /bin/bash`
7. Start simulation with `cd ~/ground-station/simulation && argos3 -c experiments/demo_pdr.argos`