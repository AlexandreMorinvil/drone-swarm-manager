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

    NUMBER_POINTS_TO_SEND = 20

    # Initialize Socket to send data
    app = Flask(__name__)
    socketio = SocketIO(app ,cors_allowed_origins='*')
    class __OnlyOne:
        def __init__(self):
            self.db = DatabaseConnector()
            self.current_map = None
            self.__databasePoint = []

        def initialize_map(self):
            time_now = datetime.now()
            datettime_string = time_now.strftime("%d/%m/%Y %H:%M:%S")
            map_id = self.db.add_map("Mission",datettime_string)
            self.current_map = Map("Mission", map_id)


        def send_base_map(self, socketio_socket):
            json_map = self.current_map.to_json()
            socketio_socket.emit('LIVE_BASE_MAP', json_map, broadcast=True)

        def send_point(self, socketio_socket):
            
            while True:
                
                # If the ap was not initialized, it is then initialized
                if not self.current_map:
                    self.initialize_map()

                # Fetch point from the newly genereated point's queue and if 
                # the point is new send it to the client and save it locally
                point = MapObservationAccumulator.provide_point()
                point_to_report = self.current_map.addPoint(point)
                if point_to_report:
                    self.__databasePoint.append(point_to_report)
                    socketio_socket.emit('LIVE_MAP_NEW_POINT', json.dumps(point.toJson()), broadcast=True)
                    self.save_point()
                
        def save_point(self):
            if len(self.__databasePoint) >= MapHandler.NUMBER_POINTS_TO_SEND :
                self.db.update_map(self.current_map.id, self.__databasePoint)
                self.__databasePoint = []

    # Initialization of the singleton map handler
    instance = None
    def __init__(self):
        if not MapHandler.instance:
            MapHandler.instance = MapHandler.__OnlyOne()

    def __getattr__(self, name):
        return getattr(self.instance, name)