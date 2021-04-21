import unittest
from unittest.mock import MagicMock
import math
import sys
import os

sys.path.append(os.path.abspath('..'))
sys.path.append(os.path.abspath('../src'))
sys.path.append(os.path.abspath('../src/map'))
from src.vec3 import Vec3
from src.drone_real import DroneReal

class TestDroneRealMethods(unittest.TestCase):
    droneReal = DroneReal("5500", Vec3(1, 2, 3))
    def testConnected(self):
        self.droneReal._connected("link_uri")
        self.assertEqual(self.droneReal._isConnected, True, "isConneted = false when connected is called")

    def testConnectionLost(self):
        self.droneReal._connection_lost("link_uri", "test")
        self.assertEqual(self.droneReal._isConnected, False, "isConneted = true when _connection_lost is called")


    def testDisconnected(self):
        self.droneReal._disconnected("link_uri")
        self.assertEqual(self.droneReal._isConnected, False, "isConneted = true when _disconnected is called")
        
if __name__ == '__main__':
    unittest.main()
