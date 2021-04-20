from vec3 import Vec3
import sys
import os
from DBconnect import DatabaseConnector

# Add paths toward dependecies in different subdirectories
sys.path.insert(1, os.path.abspath('../'))


class Map:

    RESOLUTION = 0.05

    def __init__(self, name, id, time):
        self.db = DatabaseConnector()
        self.id = id
        self.__name = name
        self.__points = {}
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

    def is_point_present(self, point):

        # Verify whether or not the point is already present in the map
        return point.x in self.__points and point.y in self.__points[point.x]

    def get_points(self):
        return [Vec3(row, column).to_json() for row in map for column in map[row]]

    def round_point(self, point):
        
        # Round the position of the point to the closest point which is a multiple of the RESOLUTION
        rounded_point = point.mul(1/Map.RESOLUTION)
        rounded_point = rounded_point.round()
        rounded_point = rounded_point.mul(Map.RESOLUTION)
        return rounded_point


    def to_json(self):
        points = self.get_points()
        return {
            name: self.__name,
            points: [point.to_json() for point in points]
        }