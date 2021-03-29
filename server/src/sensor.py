from math import pi, sin, cos
from vec3 import Vec3


class Sensor:

    RIGHT_ANGLE_OFFSET = 3 * pi / 2
    LEFT_ANGLE_OFFSET = pi / 2

    # when all 3 angular values are null :
    # the front sensor has the same direction and sens as (1,0,0)
    # the right sensor has the same direction and sens as (0,1,0)
    def __init__(self, front=0, back=0, down=0, left=0, right=0, up=0, yaw=0, pitch=0, roll=0):
        self.set_sensor_ranges(front, back, down, left, right, up)
        self.set_sensor_orientations(yaw, pitch, roll)
        self.DISTANCE_FACTOR = 0.01

    def set_sensor_ranges(self, front=0, back=0, down=0, left=0, right=0, up=0):
        self.front = front
        self.back = back
        self.down = down
        self.left = left
        self.right = right
        self.up = up

    def set_sensor_orientations(self, yaw=0, pitch=0, roll=0):
        self.yaw = yaw
        self.pitch = pitch
        self.roll = roll

    # returns coordinates of collision in the 3D landmark where (0,0,0) is the location of the drone (translated space)
    # spherical coordinates (angles and distances) -> vec3 (x,y,z)
    def getEdgeFront(self) -> Vec3:
        if self.front < 0:
            return None

        coord = Vec3(0, 0, 0)
        coord.x = round(self.front * cos(self.pitch) * cos(self.yaw), 8)
        coord.y = round(self.front * cos(self.pitch) * sin(self.yaw), 8)
        coord.z = round(self.front * sin(self.pitch), 8)
        return coord.mul(self.DISTANCE_FACTOR)

    def getEdgeLeft(self) -> Vec3:
        if self.left < 0:
            return None

        coord = Vec3(0, 0, 0)
        coord.x = -round(self.left * cos(self.roll) * sin(self.yaw), 8)
        coord.y = round(self.left * cos(self.roll) * cos(self.yaw), 8)
        coord.z = -round(self.left * sin(self.roll), 8)
        return coord.mul(self.DISTANCE_FACTOR)

    def getEdgeRight(self) -> Vec3:
        if self.right < 0:
            return None

        coord = Vec3(0, 0, 0)
        coord.x = round(self.right * cos(self.roll) * sin(self.yaw), 8)
        coord.y = -round(self.right * cos(self.roll) * cos(self.yaw), 8)
        coord.z = round(self.right * sin(self.roll), 8)
        return coord.mul(self.DISTANCE_FACTOR)

    def getEdgeBack(self) -> Vec3:
        if self.back < 0:
            return None

        coord = Vec3(0, 0, 0)
        coord.x = -round(self.back * cos(self.pitch) * cos(self.yaw), 8)
        coord.y = -round(self.back * cos(self.pitch) * sin(self.yaw), 8)
        coord.z = -round(self.back * sin(self.pitch), 8)
        return coord.mul(self.DISTANCE_FACTOR)
