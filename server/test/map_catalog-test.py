import unittest
from unittest.mock import MagicMock
import math
import sys
import os

sys.path.append(os.path.abspath('..'))
sys.path.append(os.path.abspath('../src'))
from src.vec3 import Vec3
from src.map.map_catalog import MapCatalog

class TestMapCatalogMethods(unittest.TestCase):
    def testMapListToJson(self):
        mapCatalog = MapCatalog()
        self.assertEqual(mapCatalog.map_list_to_Json(['idTest', 'newName', '2021']),
                         {'id': 'idTest', 'name': 'newName', 'date': '2021'},
                         "MAP_LIST_TO_JSON fail")

    def testMapPointsToJson(self):
        mapCatalog = MapCatalog()
        self.assertEqual(mapCatalog.map_points_toJson([0, 1, 2]),
                         {'x': 0, 'y': 1, 'z': 2},
                         "MAP_POINTS_TO_JSON fail")

      



if __name__ == '__main__':
    unittest.main()