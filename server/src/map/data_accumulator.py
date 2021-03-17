from map import Map
import socketio

import sys
import os

sys.path.insert(1, os.path.abspath('../'))


class MapObservationAccumulator:

    # Using the singleton desing pattern
    class __OnlyOne:
        def __init__(self):
            self.current_map = None
            self.points_in_making = {}

        def receive_position(self, id, x, y, z):
            if not self.points_in_making[id]:
                self.points_in_making[id] = {}

            self.points_in_making[id].x_position = x
            self.points_in_making[id].y_position = y
            self.points_in_making[id].z_position = z

        def receive_position(self, id, x, y, z):
            if not self.points_in_making[id]:
                self.points_in_making[id] = {}

            self.points_in_making[id].x_position = x
            self.points_in_making[id].y_position = y
            self.points_in_making[id].z_position = z

    # Initialization of the singleton
    instance = None

    def __init__(self):
        if not MapHandler.instance:
            MapHandler.instance = MapHandler.__OnlyOne()
        else:
            MapHandler.instance

    def __getattr__(self, name):
        return getattr(self.instance, name)
