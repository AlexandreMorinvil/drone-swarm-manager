import unittest
from unittest.mock import MagicMock
import math
import sys
import os

sys.path.append(os.path.abspath('..'))
sys.path.append(os.path.abspath('../src'))
from src.vec3 import Vec3
from src.map.data_accumulator import *

class TestMapObservationAccumulatorMethods(unittest.TestCase):
    def testReceivePositon(self):
        mapObs = MapObservationAccumulator(True)
        mapObs.make_points = MagicMock()
        mapObs.receive_position(1, 2, 3)
        self.assertEqual(mapObs.position, Vec3(1, 2, 3), "Receive_position fail with position not correct")
        self.assertEqual(mapObs.got_new_positions, True, "Receive_position fail with got_new_position = false")
        assert mapObs.make_points.called, "make_points is not called in receive_position"

    def testReceiveSensorDistances(self):
        mapObs = MapObservationAccumulator(True)
        mapObs.make_points = MagicMock()
        mapObs.sensor.set_sensor_ranges = MagicMock()
        mapObs.receive_sensor_distances(10, 10, 10, 10, 10, 10)
        self.assertEqual(mapObs.got_new_distances, True, "receive_sensor_distances fail with got_new_distances = false")
        assert mapObs.make_points.called, "make_points is not called in receive_sensor_distances"
        assert mapObs.sensor.set_sensor_ranges.called, "sensor.set_sensor_ranges not called in receive_sensor_distances"

    def testReceiveSensorOrientations(self):
        mapObs = MapObservationAccumulator(True)
        mapObs.make_points = MagicMock()
        mapObs.sensor.set_sensor_orientations = MagicMock()
        mapObs.receive_sensor_orientations(10, 10, 10)
        self.assertEqual(mapObs.got_new_orientations, True, "receive_sensor_orientations fail with got_new_orientations = false")
        assert mapObs.make_points.called, "make_points is not called in receive_sensor_orientations"
        assert mapObs.sensor.set_sensor_orientations.called, "sensor.set_sensor_orientations not called in receive_sensor_orientations"

    def testMakePointsWithOldPositions(self):
        mapObs = MapObservationAccumulator(True)
        mapObs.add_points = MagicMock()
        mapObs.got_new_positions = False
        mapObs.make_points()
        assert not mapObs.add_points.called, "add_point is not called in make_points with got_new_positions = false"

    def testMakePointsWithOldOrientation(self):
        mapObs = MapObservationAccumulator(True)
        mapObs.add_points = MagicMock()
        mapObs.got_new_orientations = False
        mapObs.make_points()
        assert not mapObs.add_points.called, "add_point is not called in make_points with got_new_orientations = false"

    def testMakePointsWithNewAll(self):
        mapObs = MapObservationAccumulator(True)
        mapObs.add_points = MagicMock()
        mapObs.got_new_orientations = True
        mapObs.got_new_distances = True
        mapObs.got_new_positions = True
        mapObs.make_points()
        assert mapObs.add_points.called, "add_point is called in make_points with new all"

if __name__ == '__main__':
    unittest.main()
