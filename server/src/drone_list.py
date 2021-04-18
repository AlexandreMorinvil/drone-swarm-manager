import json
from drone import Drone

class DroneList:

    drone_list = []
    is_drone_list_initialized = False

    def __init__(self, drone_list = []):
        if not DroneList.is_drone_list_initialized:
            DroneList.drone_list = drone_list

    def add_drone(drone):
        DroneList.drone_list.append(drone)

    def dumps(self):
        json.dumps([drone.dump() for drone in self.drone_list])
