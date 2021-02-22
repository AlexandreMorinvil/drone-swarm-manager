import unittest
import math
from sensor import Sensor
from vec3 import Vec3
rt18 = math.sqrt(18)
easySensor = Sensor(10,10,10,10,10,10,0,0,0) # all angles are null
midSensor  = Sensor(rt18,rt18,rt18,rt18,rt18,rt18,math.pi/4,0,0)

class TestSensorMethods(unittest.TestCase) :
    def testGetEdgeFront(self) :
        self.assertEqual(easySensor.getEdgeFront(),Vec3(10,0,0),"FRONT: easy fail")
        self.assertEqual(midSensor.getEdgeFront(),Vec3(3,3,0), "FRONT: mid fail")
    def testGetEdgeRight(self) :
        self.assertEqual(easySensor.getEdgeRight(),Vec3(0,10,0),"RIGHT: easy fail")
        self.assertEqual(midSensor.getEdgeRight(),Vec3(-3,3,0),"RIGHT: mid fail")
    def testGetEdgeLeft(self) :
        self.assertEqual(easySensor.getEdgeLeft(),Vec3(0,-10,0),"LEFT: easy fail")
        self.assertEqual(midSensor.getEdgeLeft(),Vec3(3,-3,0),"LEFT: mid fail")
if __name__ == '__main__':
    unittest.main()