import math
from vec3 import Vec3


class Sensor:
    up = 0  # upwards sensor
    down = 0  # downwards sensor (altitude)
    left = 0  # left sensor
    right = 0  # right sensor
    front = 0  # front sensor
    back = 0  # back sensor
    # when all 3 angular values are null :
    # the front sensor has the same direction and sens as (1,0,0)
    # the right sensor has the same direction and sens as (0,1,0)
    yaw = 0  # Z axis rotation (rad)
    pitch = 0  # Y axis rotation (rad)
    roll = 0  # X axis rotation (rad)

    def __init__(self, up=0, down=0, left=0, right=0, front=0, back=0, yaw=0, pitch=0, roll=0):
        self.set_sensor_ranges(up, down, left, right, front, back)
        self.set_sensor_orientations(yaw, pitch, roll)

    def set_sensor_ranges(self, up=0, down=0, left=0, right=0, front=0, back=0):
        self.up = up
        self.down = down
        self.left = left
        self.right = right
        self.front = front
        self.back = back

    def set_sensor_orientations(self, yaw=0, pitch=0, roll=0):
        self.yaw = yaw
        self.pitch = pitch
        self.roll = roll

    # returns coordinates of collision in the 3D landmark where (0,0,0) is the location of the drone (translated space)
    # spherical coordinates (angles and distances) -> vec3 (x,y,z)
    def getEdgeFront(self) -> Vec3:
        coord = Vec3(0, 0, 0)
        coord.x = round(self.front * math.cos(self.pitch) * math.cos(self.yaw), 8)
        coord.z = round(self.front * math.sin(self.pitch), 8)
        coord.y = round(self.front * math.cos(self.pitch) * math.sin(self.yaw), 8)
        return coord

    def getEdgeLeft(self) -> Vec3:
        coord = Vec3(0, 0, 0)
        coord.x = round(self.left * math.cos(self.roll) * math.sin(self.yaw), 8)
        coord.y = round(self.left * math.cos(self.roll) * math.cos(self.yaw) * -1, 8)
        coord.z = round(self.left * math.sin(self.roll), 8)
        return coord

    def getEdgeRight(self) -> Vec3:
        coord = Vec3(0, 0, 0)
        coord.x = round(self.left * math.cos(self.roll) * math.sin(self.yaw), 8)
        coord.y = round(self.left * math.cos(self.roll) * math.cos(self.yaw), 8)
        coord.z = round(self.left * math.sin(self.roll), 8)
        return coord

    def getEdgeBack(self) -> Vec3:
        coord = Vec3(0, 0, 0)
        coord.x = round(self.back * math.cos(self.pitch) * math.cos(self.yaw), 8)
        coord.z = round(self.back * math.sin(self.pitch), 8)
        coord.y = round(self.back * math.cos(self.pitch) * math.sin(self.yaw), 8)
        return coord

    def toJson(self):
        return {
            'up   ': self.up,
            'down ': self.down,
            'left ': self.left,
            'right': self.right,
            'front': self.front,
            'back ': self.back,
            'yaw  ': self.yaw,
            'pitch': self.pitch,
            'roll ': self.roll,
            'detectedR': self.getEdgeRight().toJson(),
            'detectedL': self.getEdgeLeft().toJson(),
            'detectedF': self.getEdgeFront().toJson()
        }
