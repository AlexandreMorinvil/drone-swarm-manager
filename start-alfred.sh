#!/bin/bash
cd server && pipenv install && cd src && pipenv run python main.py &
cd client && npm start &
./simulation/launch_scripts/launch_container.sh &
