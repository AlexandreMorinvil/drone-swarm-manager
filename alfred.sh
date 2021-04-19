# Launch the ARGoS simulation
./simulation/launch_scripts/launch_container.sh 

# Launch docker compose file with the client and the server software
docker-compose -f "docker-compose.yml" up -d --build