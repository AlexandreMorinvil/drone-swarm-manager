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

    easySensor = Sensor(False, 10, 10, 10, 10, 10, 10, 0, 0, 0)  # all angles are null
    midSensor = Sensor(False, rt18, rt18, rt18, rt18, rt18, rt18, math.pi / 4, 0, 0)
    easySensorSimul = Sensor(True, 10, 10, 10, 10, 10, 10, 0, 0, 0)  # all angles are null
    midSensorSimul = Sensor(True, rt18, rt18, rt18, rt18, rt18, rt18, math.pi / 4, 0, 0)

    # Option drones
    def testGetEdgeFront(self):
        self.assertEqual(self.easySensor.getEdgeFront(), Vec3(0.01, 0.0, 0.0), "FRONT: easy fail")
        self.assertEqual(self.midSensor.getEdgeFront(), Vec3(0.003, 0.003, 0.0), "FRONT: mid fail : ")

    def testGetEdgeRight(self):
        self.assertEqual(self.easySensor.getEdgeRight(), Vec3(0, -0.01, 0), "RIGHT: easy fail")
        self.assertEqual(self.midSensor.getEdgeRight(), Vec3(0.003, -0.003, 0), "Right: midSensor :")

    def testGetEdgeLeft(self):
        self.assertEqual(self.easySensor.getEdgeLeft(), Vec3(0, 0.01, 0), "LEFT: easy fail")
        self.assertEqual(self.midSensor.getEdgeLeft(), Vec3(-0.003, 0.003, 0), "LEFT: mid fail : ")
 
    def testGetEdgeBack(self):
        self.assertEqual(self.easySensor.getEdgeBack(), Vec3(-0.01, 0, 0), "BACK: easy fail")
        self.assertEqual(self.midSensor.getEdgeBack(), Vec3(-0.003, -0.003, 0), "BACK: mid fail : ")

    # Option simulation
    def testGetEdgeFrontSimul(self):
        self.assertEqual(self.easySensorSimul.getEdgeFront(), Vec3(0.1, 0.0, 0.0), "FRONT SIMULATION: easy fail")
        self.assertEqual(self.midSensorSimul.getEdgeFront(), Vec3(0.03, 0.03, 0.0), "FRONT SIMULATION: mid fail : ")

    def testGetEdgeRightSimul(self):
        self.assertEqual(self.easySensorSimul.getEdgeRight(), Vec3(0, -0.1, 0), "RIGHT SIMULATION: easy fail")
        self.assertEqual(self.midSensorSimul.getEdgeRight(), Vec3(0.03, -0.03, 0), "RIGHT SIMULATION: midSensor :")

    def testGetEdgeLeftSimul(self):
        self.assertEqual(self.easySensorSimul.getEdgeLeft(), Vec3(0, 0.1, 0), "LEFT SIMULATION: easy fail")
        self.assertEqual(self.midSensorSimul.getEdgeLeft(), Vec3(-0.03, 0.03, 0), "LEFT SIMULATION: mid fail : ")
  
    def testGetEdgeBackSimul(self):
        self.assertEqual(self.easySensorSimul.getEdgeBack(), Vec3(-0.1, 0, 0), "BACK SIMULATION: easy fail")
        self.assertEqual(self.midSensorSimul.getEdgeBack(), Vec3(-0.03, -0.03, 0), "BACK SIMULATION: mid fail : ")

if __name__ == '__main__':
    unittest.main()
