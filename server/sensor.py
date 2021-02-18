import math
from .server import Vec3
class Sensor :
    up    = 0 # upwards sensor
    down  = 0 # downwards sensor (altitude)
    left  = 0 # left sensor
    right = 0 # right sensor
    front = 0 # front sensor
    back  = 0 # back sensor
    yaw   = 0 # Z axis rotation (rad)
    pitch = 0 # Y axis rotation (rad)
    roll  = 0 # X axis rotation (rad)
    def __init__(self, u,d,l,r,f,b):
        self.up    = u
        self.down  = d
        self.left  = l
        self.right = r
        self.front = f
        self.back  = b

    # returns coordinates of collision in the 3D landmark where (0,0,0) is the location of the drone (translated space)
    # polar coordinates (angles and distances) -> vec3 (x,y,z)
    def getEdgeFront(self) -> Vec3:
        coord = Vec3()
        coord.x = self.front * math.sin(self.pitch) * math.cos(self.yaw)
        coord.y = self.front * math.sin(self.pitch) * math.sin(self.yaw)
        coord.z = self.front * math.cos(self.pitch)
        return coord

    def getEdgeLeft(self) -> Vec3 :
        coord = Vec3()
        coord.x = self.left * math.sin(self.roll) * math.cos(self.yaw)
        coord.y = self.left * math.sin(self.roll) * math.sin(self.yaw)
        coord.z = self.left * math.cos(self.roll)
        return coord

    def getEdgeRight(self)-> Vec3 :
        coord = Vec3()
        coord.x = self.right * math.sin(-self.roll) * math.cos(self.yaw)
        coord.y = self.right * math.sin(-self.roll) * math.sin(self.yaw)
        coord.z = self.right * math.cos(-self.roll)
        return coord
