import unittest
import math
import sys
import os

sys.path.append(os.path.abspath('..'))
sys.path.append(os.path.abspath('../src'))
from src.sensor import Sensor
from src.vec3 import Vec3

rt18 = math.sqrt(18)
rt36 = math.sqrt(36)


class TestSensorMethods(unittest.TestCase):
    def testGetEdgeFront(self):
        easySensor = Sensor(10, 10, 10, 10, 10, 10, 0, 0, 0)  # all angles are null
        midSensor = Sensor(rt18, rt18, rt18, rt18, rt18, rt18, math.pi / 4, 0, 0)
        self.assertEqual(easySensor.getEdgeFront(), Vec3(10, 0, 0), "FRONT: easy fail")
        self.assertEqual(midSensor.getEdgeFront(), Vec3(3, 3, 0), "FRONT: mid fail : ")

    def testGetEdgeRight(self):
        easySensor = Sensor(10, 10, 10, 10, 10, 10, 0, 0, 0)  # all angles are null
        midSensor = Sensor(rt18, rt18, rt18, rt18, rt18, rt18, math.pi / 4, 0, 0)
        self.assertEqual(easySensor.getEdgeRight(), Vec3(0, 10, 0), "RIGHT: easy fail")
        self.assertEqual(midSensor.getEdgeRight(), Vec3(3, 3, 0), "Right: midSensor :")

    def testGetEdgeLeft(self):
        easySensor = Sensor(10, 10, 10, 10, 10, 10, 0, 0, 0)  # all angles are null
        midSensor = Sensor(rt18, rt18, rt18, rt18, rt18, rt18, math.pi / 4, 0, 0)
        self.assertEqual(easySensor.getEdgeLeft(), Vec3(0, -10, 0), "LEFT: easy fail")
        self.assertEqual(midSensor.getEdgeLeft(), Vec3(3, -3, 0), "LEFT: mid fail : ")


if __name__ == '__main__':
    unittest.main()
