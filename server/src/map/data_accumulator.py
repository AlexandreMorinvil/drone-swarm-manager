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
        # Data structures used to generate the points
        self.position = Vec3()
        self.sensor = Sensor()

        # Track the value updates
        self.got_new_positions = False
        self.got_new_distances = False
        self.got_new_orientations = False

    def receive_position(self, x, y, z):
        self.position = Vec3(x, y, z)
        self.got_new_positions = True
        self.make_points()

    def receive_sensor_distances(self, front, back, up, left, right, zrange):
        self.sensor.set_sensor_ranges(front, back, up, left, right, zrange)
        self.got_new_distances = True
        self.make_points()

    def receive_sensor_orientations(self, yaw, pitch, roll):
        self.sensor.set_sensor_orientations(yaw, pitch, roll)
        self.got_new_orientations = True
        self.make_points()

    def make_points(self):
        if not(self.got_new_positions and self.got_new_distances and  self.got_new_orientations) :
            return

        edge_front  = self.sensor.getEdgeFront()
        edge_left   = self.sensor.getEdgeLeft()
        edge_right  = self.sensor.getEdgeRight()
        edge_back   = self.sensor.getEdgeBack()

        point_front = (self.position + edge_front) if edge_front is not None else None
        point_back  = (self.position + edge_back ) if edge_back is not None  else None
        point_left  = (self.position + edge_left ) if edge_left is not None  else None
        point_right = (self.position + edge_right) if edge_right is not None else None

        self.got_new_positions = False
        self.got_new_distances = False
        self.got_new_orientations = False

        self.add_points(point_front, point_back, point_left, point_right)

    def add_points(self, point_front, point_back, point_left, point_right):
        MapObservationAccumulator.provide_lock.acquire()
        # MapObservationAccumulator.queue.put(self.position)
        # MapObservationAccumulator.queue.put(point_front) if point_front is not None else None
        # MapObservationAccumulator.queue.put(point_back)  if point_back is not None  else None 
        # MapObservationAccumulator.queue.put(point_left)  if point_left is not None  else None 
        MapObservationAccumulator.queue.put(point_right) if point_right is not None else None
        MapObservationAccumulator.provide_lock.release()

    @classmethod
    def provide_point(cls):
        # cls.consume_lock.acquire()
        last_point = cls.queue.get()
        # cls.consume_lock.release()
        return last_point
