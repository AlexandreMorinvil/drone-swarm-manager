import unittest
import math
import sys
import os

sys.path.append(os.path.abspath('..'))
sys.path.append(os.path.abspath('../src'))
from src.vec3 import Vec3


class TestVecMethods(unittest.TestCase):
    def testEqual(self):
        self.assertTrue(Vec3(33, 32, 66) == Vec3(33, 32, 66))
        self.assertFalse(Vec3(85, 22, 54) == Vec3(467, 43, 21))

    def testAdd(self):
        foo = Vec3(0, 2, 3)
        bar = Vec3(5, 3, 2)
        self.assertEqual(foo + bar, Vec3(5, 5, 5))

    def testSub(self):
        foo = Vec3(0, 2, 3)
        bar = Vec3(5, 3, 2)
        self.assertEqual(bar - foo, Vec3(5, 1, -1))

    def testMul(self):
        foo = Vec3(1, 1, 1)
        self.assertEqual(foo.mul(3), Vec3(3, 3, 3))


if __name__ == '__main__':
    unittest.main()
