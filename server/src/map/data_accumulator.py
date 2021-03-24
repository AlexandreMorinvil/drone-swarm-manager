from map import Map
from vec3 import Vec3
from sensor import Sensor
from threading import Lock
from queue import Queue

import sys
import os

# Add paths toward dependecies in different subdirectories
sys.path.append(os.path.abspath('./src'))


class MapObservationAccumulator:
    
    # Initialization of a queue
    queue = Queue()
    provide_lock = Lock()
    consume_lock = Lock()

    # Using the singleton desing pattern
    def __init__(self):
        self.position = Vec3()
        self.point_front = Vec3()
        self.point_back = Vec3()
        self.point_left = Vec3()
        self.point_right = Vec3()
        self.sensor = Sensor()

    def receive_position(self, x, y, z):
        self.position = Vec3(x, y, z)

    def receive_sensor_distances(self, front, back, up, left, right, zrange):
        self.sensor.set_sensor_ranges(front, back, up, left, right, zrange)
        print(front, back, up, left, right, zrange)

    def receive_sensor_orientations(yaw, pitch, roll):
        self.sensior.set_sensor_orientations(yaw, pitch, roll)

    def make_points():
        self.point_front = self.position + self.sensor.getEdgeFront()
        self.point_back = self.position + self.sensor.getEdgeLeft()
        self.point_left = self.position + self.sensor.getEdgeRight()
        self.point_right = self.position + self.sensor.getEdgeBack()

    def add_points():
        self.make_points()
        MapObservationAccumulator.provide_lock.acquire()
        MapObservationAccumulator.queue.put(self.point_front)
        MapObservationAccumulator.queue.put(self.point_back)
        MapObservationAccumulator.queue.put(self.point_left)
        MapObservationAccumulator.queue.put(self.point_right)
        MapObservationAccumulator.provide_lock.release()

    def provide_point():
        MapObservationAccumulator.consume_lock.acquire()
        last_point = MapObservationAccumulator.queue.get()
        MapObservationAccumulator.consume_lock.release()
        return last_point
