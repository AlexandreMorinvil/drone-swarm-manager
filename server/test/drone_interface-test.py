import unittest
import math
import sys
import os

sys.path.append(os.path.abspath('..'))
sys.path.append(os.path.abspath('../src'))
from src.vec3 import Vec3
from src.drone_interface import DroneInterface

# For test abstract class DroneInterface
class ConcreteDroneInterface(DroneInterface):
    def send_data(self, packet, format_packer):
        print('Test')

class TestDroneInterfaceMethods(unittest.TestCase):
    droneInterface = ConcreteDroneInterface('port', Vec3(10, 10, 10))
    
    def testDump(self):
        data = self.droneInterface.dump()
        print(data)
        dataExpected = {'id': 0,
                'state': 0,
                'vbat': 10,
                'isConnected': False,
                'currentPos': Vec3(10,10,10).toJson(),
                'currentSpeed': Vec3(0,0,0).toJson(),
                }
        self.assertEqual(data, dataExpected, "DUMP fail")

if __name__ == '__main__':
    unittest.main()
