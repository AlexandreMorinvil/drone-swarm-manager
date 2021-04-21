# Add paths toward dependecies in different subdirectories
import os
import sys
sys.path.append(os.path.abspath('./drone'))
sys.path.append(os.path.abspath('./log'))

# Add dependencies
from drone_list import DroneList
from environment import Environment
from setup_logging import LogsConfig

logsConfig = LogsConfig()
logger = logsConfig.logger('EnvironmentApi')

def api_environment_set_mode(data):
    mode = data['mode_chosen']
    number_drones = data['number_of_drone']

    DroneList.delete_drones()
    Environment.set_mode(mode)

    if (Environment.is_in_simulation()):
        DroneList.createDrones(int(number_drones), mode)
        Environment.launch_simulation(number_drones)
    else:
        DroneList.createDrones(int(number_drones))

def api_environment_set_real_position(data):
    DroneList.initial_posisitions.clear()
    DroneList.initial_posisitions.extend(data)