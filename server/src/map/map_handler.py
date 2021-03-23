from map.map import Map
import socketio
from vec3 import Vec3
from DBconnect import DatabaseConnector

import sys
import os

sys.path.insert(1, os.path.abspath('../'))

# Using the singleton desing pattern
class MapHandler:
    class __OnlyOne:
        def __init__(self):
            self.db = DatabaseConnector()
            self.t = None
            self.current_map = None
            self.points_in_making = {}
            self.x_position = 0
            self.y_position = 0
            self.z_position = 0

        def initialize_map(self):
            mapId = self.db.add_map("mission")
            self.current_map = Map("mission", mapId)


        def send_base_map(self):
            json_map = self.current_map.toJson()
            socketio.emit('LIVE_BASE_MAP', json_map, broadcast=True)

        def send_point(self, point):
            socketio.socketio.emit('LIVE_MAP_NEW_POINT', point, broadcast=True)

        def receive_position(self, id, x, y, z):
            self.x_position = x
            self.y_position = y
            self.z_position = z

            



    # Initialization of the singleton
    instance = None
    def __init__(self):
        if not MapHandler.instance:
            MapHandler.instance = MapHandler.__OnlyOne()
        else:
            MapHandler.instance

    def __getattr__(self, name):
        return getattr(self.instance, name)