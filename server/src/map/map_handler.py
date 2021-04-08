from map import Map
import json
import socketio
from flask import Flask
from flask_socketio import *

from vec3 import Vec3
from DBconnect import DatabaseConnector
from datetime import datetime
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
            self.db = DatabaseConnector()
            self.t = None
            self.current_map = None
            self.is_consuming = True
            self.__databasePoint = []

        def initialize_map(self):
            now = datetime.now()
            dt_string = now.strftime("%d/%m/%Y %H:%M:%S")
            print(dt_string)
            mapId = self.db.add_map("Mission",dt_string )
            self.current_map = Map("Mission", mapId)


        def send_base_map(self):
            json_map = self.current_map.toJson()
            MapHandler.socketio.emit('LIVE_BASE_MAP', json_map, broadcast=True)

        def send_point(self, socketio_socket):
            while self.is_consuming:
                point = MapObservationAccumulator.provide_point()
                self.__databasePoint.append(point)
                socketio_socket.emit('LIVE_MAP_NEW_POINT', json.dumps(point.toJson()), broadcast=True)
                self.save_point()
                
        def save_point(self):
            if self.current_map == None:
                self.initialize_map()
            if len(self.__databasePoint) >= 20 :
                self.db.update_map(self.current_map.id, self.__databasePoint)
                self.__databasePoint = []

    # Initialization of the singleton
    instance = None
    def __init__(self):
        if not MapHandler.instance:
            MapHandler.instance = MapHandler.__OnlyOne()
        else:
            MapHandler.instance

    def __getattr__(self, name):
        return getattr(self.instance, name)