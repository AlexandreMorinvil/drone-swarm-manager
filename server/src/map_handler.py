import math
from vec3 import Vec3


class Sensor:


    def __init__(self, up, down, left, right, front, back, yaw, pitch, roll):
        self.up = up
        self.down = down
        self.left = left
        self.right = right
        self.front = front
        self.back = back
        self.yaw = yaw
        self.pitch = pitch
        self.roll = roll

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
