import json
from drone import Drone

# Usage of a signe
class DroneList:
    class __OnlyOne:
        def __init__(self, arg = []):
            self.drone_list = arg

        def dumps(self):
            json.dumps([drone.dump() for drone in self.drone_list])

    instance = None
    def __init__(self, drone_list = []):
        if not DroneList.instance:
            DroneList.instance = DroneList.__OnlyOne(drone_list)
        else:
            DroneList.instance.drone_list = drone_list

    def __getattr__(self, name):
        return getattr(self.instance, name)