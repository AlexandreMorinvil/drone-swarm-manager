# Add paths toward dependecies in different subdirectories
import os
import sys
sys.path.append(os.path.abspath('./log'))

# Add dependencies
import threading
import json
from setup_logging import LogsConfig
from environment import Environment
from drone_real import DroneReal
from drone_simulation import DroneSimulation
from drone_interface import DroneInterface, StateMode
from vec3 import Vec3

# Add dependecies

# Singleton Drone list


class DroneList:

    # Singleton attributes
    drones = []
    initial_posisitions = []
    default_port = 5015
    is_drone_list_initialized = False

    # Logger attribute
    logsConfig = None
    logger = None

    def __init__(self, drone_list=[]):
        if not DroneList.is_drone_list_initialized:
            # Initialize the drone list
            DroneList.drone_list = drone_list

            # Initialize the logger
            DroneList.logsConfig = LogsConfig()
            DroneList.logger = self.logsConfig.logger('DroneList')

            # Indicate the initialization as completed
            DroneList.is_drone_list_initialized = True

    @classmethod
    def dumps(cls):
        return json.dumps([drone.dump() for drone in cls.drones])

    @classmethod
    def createDrones(cls, number_drones, mode, initial_posisitions=None):
        for i in range(number_drones):
            if Environment.is_in_simulation():
                cls.drones.append(DroneSimulation(cls.default_port + i))
                # print(cls.drones)
                t = threading.Thread(
                    target=cls.drones[i].waiting_connection, name='waiting_connection')
                t.start()
                cls.logger.info(
                    'Connection to port {}'.format(cls.default_port + i))
            else:
                initial_posisitions_vec3 = Vec3(initial_posisitions[i]['x'], initial_posisitions[i]['y'])
                cls.drones.append(
                    DroneReal(initial_posisitions[i]['address'], initial_posisitions_vec3))

    @classmethod
    def delete_drones(cls):
        """Disconnect the drones brefore resetting the list"""
        for drone in cls.drones:
            drone.delete()
        cls.drones.clear()
        cls.logger.info('Deleted all drones from the drone list')

    @classmethod
    def get_drone_from_id(cls, id):
        return cls.drones[id - DroneInterface.id_counter]

    @classmethod
    def get_number_drones(cls):
        return drones.length
