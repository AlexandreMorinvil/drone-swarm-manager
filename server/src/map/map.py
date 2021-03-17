from vec3 import Vec3
import math
import sys
import os

# Add paths toward dependecies in different subdirectories
sys.path.insert(1, os.path.abspath('../'))

class Map:
    def __init__(self, name="Map Name"):
        self.__name = name
        self.__points = []

    def addPoint(self, point):
        self.__points.append(point)

    def setBaseMap(self, points, name="Map Name"):
        self.__name =
        self.__points = points

    def toJson(self):
        return {
            name: self.__name,
            points: self.__points
        }
