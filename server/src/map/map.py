from vec3 import Vec3
import math
import sys
import os
from DBconnect import DatabaseConnector

# Add paths toward dependecies in different subdirectories
sys.path.insert(1, os.path.abspath('../'))


class Map:

    RESOLUTION = 0.05

    def __init__(self, name, id):
        self.db = DatabaseConnector()
        self.__name = name
        self.__points = {}
        self.id = id
        self._endMission = False

    def addPoint(self, point):
        
        # If the rounded point is present, we ignore the point
        point_to_add = self.round_point(point)
        if self.is_point_present(point_to_add):
            return None

        # If the point is a new point, we add it to the map structure
        if not point_to_add.x in self.__points:
            self.__points[point_to_add.x] = set()  
        self.__points[point_to_add.x].add(point_to_add.y)
        
        # Return the point saved
        return point_to_add

    def round_point(self, point):
        rounded_point = point.mul(1/Map.RESOLUTION)
        rounded_point = rounded_point.round()
        rounded_point = rounded_point.mul(Map.RESOLUTION)
        return rounded_point

    def is_point_present(self, point):
        return point.x in self.__points and point.y in self.__points[point.x]

    def get_points(self):
        return [Vec3(row, column) for row in map for column in map[row]]


    def to_json(self):
        return {
            name: self.__name,
            points: self.get_points()
        }

    def end_mission(self):
        self.db.update_map(self.id, self.__databasePoint)
