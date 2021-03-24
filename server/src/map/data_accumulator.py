from map import Map
from vec3 import Vec3
from sensor import Sensor
from threading import Lock
from queue import Queue 

import sys
import os

# Add paths toward dependecies in different subdirectories
sys.path.append(os.path.abspath('./src'))
from sensor import Sensor


class MapObservationAccumulator:

    # Using the singleton desing pattern
    class __OnlyOne:
        def __init__(self):
            self.queue = Queue()
            self.position = Vec3()
            self.point_front = Vec3()
            self.point_back = Vec3()
            self.point_left = Vec3()
            self.point_right = Vec3()

            self.sensor = Sensor()
            self.lock = Lock()

        def receive_position(self, x, y, z):
            self.position = Vec3(x, y, z)

        def receive_sensor_distances(self, front, back, up, left, right, zrange):
            self.sensor.set_sensor_ranges(front, back, up, left, right, zrange)

        def receive_sensor_orientations(yaw, pitch, roll):
            self.sensior.set_sensor_orientations(yaw, pitch, roll)

        def make_points():
            self.point_front = self.sensor.getEdgeFront()
            self.point_back = self.sensor.getEdgeLeft()
            self.point_left = self.sensor.getEdgeRight()
            self.point_right = self.sensor.getEdgeBack()

        def add_point(point):
            self.lock.acquire()
            self.queue.put(point)
            self.lock.release()

        def provide_point():
            self.lock.acquire()
            last_point =  self.queue.get()
            self.lock.release()
            return last_point

    # Initialization of the singleton
    instance = None
    def __init__(self):
        if not MapObservationAccumulator.instance:
            MapObservationAccumulator.instance = MapObservationAccumulator.__OnlyOne()
        else:
            MapObservationAccumulator.instance

    def __getattr__(self, name):
        return getattr(self.instance, name)
