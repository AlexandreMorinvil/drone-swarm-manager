
if [ $1 == 'on' ]
then
    # Launch docker compose file with the client and the server software
    docker-compose -f "docker-compose.yml" up -d --build

    # Launch the ARGoS simulation
    ./simulation/launch_scripts/launch_container.sh 
elif [ $1 == 'off' ]
then
    # Stop the docker compose file with the client and the server software
    ./simulation/launch_scripts/stop_container.sh

    # Stop the docker compose file with the client and the server software
    docker-compose -f "docker-compose.yml" down
fi

