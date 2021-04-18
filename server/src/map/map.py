from vec3 import Vec3
import math
import sys
import os
from DBconnect import DatabaseConnector

# Add paths toward dependecies in different subdirectories
sys.path.insert(1, os.path.abspath('../'))

class Map:

    RESOLUTION = 0.02

    def __init__(self, name, id):
        self.db = DatabaseConnector()
        self.__name = name
        self.__points = []
        self.id = id
        self._endMission = False
        

    def addPoint(self, point):

        self.__points.append(point)

    def setBaseMap(self, points, name="Map Name"):
        self.__name = name
        self.__points = points

    def toJson(self):
        return {
            name: self.__name,
            points: self.__points
        }

    def end_mission(self):
        self.db.update_map(self.id, self.__databasePoint)

