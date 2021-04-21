# Add paths toward dependecies in different subdirectories
import os
import sys
sys.path.append(os.path.abspath('./drone'))
sys.path.append(os.path.abspath('./log'))

# Add dependencies
from drone_interface import StateMode
from drone_list import DroneList
from setup_logging import LogsConfig

logsConfig = LogsConfig()
logger = logsConfig.logger('ApiControl')

# To change the state of drones
def api_control_switch_state(data):
    id = data['id']
    state = data['state']

    if (data['id'] == -2):
        for drone in DroneList.drones:
            drone.switch_state(state)
            logger.info('Switch state of drone #{} to {} mode'.format(id, StateMode(state)))
    else:
        drone = DroneList.get_drone_from_id(id)
        drone.switch_state(state)
        logger.info('Switch state of drone #{} to {} mode'.format(id, StateMode(state)))