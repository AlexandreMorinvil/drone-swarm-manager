import socketio
from flask import Flask
from flask_socketio import *

from map import Map
from vec3 import Vec3
from data_accumulator import MapObservationAccumulator

import sys
import os

# sys.path.append(os.path.abspath('../'))

# Using the singleton desing pattern
class MapHandler:

    # Initialize Socket to send data
    app = Flask(__name__)
    socketio = SocketIO(app ,cors_allowed_origins='*')
    class __OnlyOne:
        def __init__(self):
            self.current_map = None
            self.is_consuming = True

        def initialize_map():
            self.current_map = Map()

        def send_base_map(self):
            json_map = self.current_map.toJson()
            MapHandler.socketio.emit('LIVE_BASE_MAP', json_map, broadcast=True)

        def send_point(self):
            while self.is_consuming:
                point = MapObservationAccumulator.provide_point()
                print("SO FAR SO GOOD: ", point.x, point.y, point.z)
                MapHandler.socketio.emit('LIVE_MAP_NEW_POINT', point, broadcast=True)

    # Initialization of the singleton
    instance = None
    def __init__(self):
        if not MapHandler.instance:
            MapHandler.instance = MapHandler.__OnlyOne()
        else:
            MapHandler.instance

    def __getattr__(self, name):
        return getattr(self.instance, name)