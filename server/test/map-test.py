import unittest
from unittest.mock import MagicMock
import math
import sys
import os

sys.path.append(os.path.abspath('..'))
sys.path.append(os.path.abspath('../src'))
from src.vec3 import Vec3
from src.map.map import Map

class TestMapMethods(unittest.TestCase):
    def testToJson(self):
        mapTest = Map('mapTest', 'idTest')
        result = {
            'name': 'mapTest',
            'points': []
        }
        self.assertEqual(mapTest.toJson(), result, "toJson fail")
    
    def testAddPoint(self):
        mapTest = Map('mapTest', 'idTest')
        point = Vec3(1, 2, 3)
        mapTest.addPoint(point)
        result = {
            'name': 'mapTest',
            'points': [Vec3(1, 2, 3)]
        }
        self.assertEqual(mapTest.toJson(), result, "AddPoint fail")

    def testSetBaseMap(self):
        mapTest = Map('mapTest', 'idTest')
        points = [1, 2, 3]
        mapTest.setBaseMap(points, 'newName')
        result = {
            'name': 'newName',
            'points': [1, 2, 3]
        }
        self.assertEqual(mapTest.toJson(), result, "SetBaseMap fail")
    
if __name__ == '__main__':
    unittest.main()
