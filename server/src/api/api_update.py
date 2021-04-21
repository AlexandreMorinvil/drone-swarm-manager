# Add paths toward dependecies in different subdirectories
import os
import sys
sys.path.append(os.path.abspath('./drone'))
sys.path.append(os.path.abspath('./log'))

# Add dependencies
from drone_interface import StateMode
from drone_list import DroneList
from setup_logging import LogsConfig
from subprocess import call

logsConfig = LogsConfig()
logger = logsConfig.logger('ApiControl')

def api_update_receive_file(data):
    name = data['name']
    content = data['content']

    file = open('../../robot/src/' + name, 'wb')
    file.write(content)

def api_update_start_update():
    # Verify that all drones are in stand-by
    for drone in DroneList.drones:
        if (drone.get_state() != StateMode.STANDBY.value):
            return False

    # Perform update    
    number_drones = len(DroneList.drones)
    DroneList.delete_drones()
    for address in DroneList.initial_posisitions:
        if (call(['scripts/update-robot.sh', '{}'.format(address['address'])]) == 1):
            return False

    # Recreate drones        
    DroneList.createDrones(number_drones)
    return True

def api_update_send_sources():
    for root, dirs, files in os.walk('../../robot/src/'):
        for filename in files:
            f = open('../../robot/src/'+filename)
            return {
                'name': filename,
                'data': f.read()
            }

