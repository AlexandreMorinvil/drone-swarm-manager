# Add paths toward dependecies in different subdirectories
import os
import sys
sys.path.append(os.path.abspath('./log'))

# Add dependecies
import json
import threading
from drone_interface import StateMode
from drone_simulation import DroneSimulation
from drone_real import DroneReal
from environment import Environment
from setup_logging import LogsConfig

# Singleton Drone list
class DroneList:

    # Singleton attributes
    drones = []
    initPos = []
    default_port = 5015
    is_drone_list_initialized = False

    # Logger attribute
    logsConfig = None
    logger = None

    def __init__(self, drone_list = []):
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
        json.dumps([drone.dump() for drone in cls.drones])

    @classmethod
    def createDrones(cls, number_drones, mode):
        print("================================== ON SE REND ICI ==================================")
        for i in range(number_drones):
            print("1")
            if Environment.mode == Mode.REAL:
                print("== REAL")
                initPosVec3 = Vec3(initPos[i]['x'], initPos[i]['y'])
                drones.append(DroneReal(initPos[i]['address'], initPosVec3))
            else:
                print("== Else")
                drones.append(DroneSimulation(cls.default_port + i))
                t = threading.Thread(
                    target=drones[i].waiting_connection, name='waiting_connection')
                t.start()
                cls.logger.info('Connection to port {}'.format(cls.default_port + i))

    @classmethod
    def delete_drones(cls):
        """Disconnect the drones brefore resetting the list"""
        for drone in cls.drones:
            del drone
        cls.drones.clear()
        cls.logger.info('Deleted all drones from the drone list')

    @classmethod
    def activate_drones(cls):
        return drones.length

    @classmethod
    def get_number_drones(cls):
        return drones.length