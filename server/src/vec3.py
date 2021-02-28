# vecteur 3d
class Vec3:
    x = 0
    y = 0
    z = 0

    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def __add__(self, other):
        return Vec3(self.x + other.x, self.y + other.y, self.z + other.z)

    def __sub__(self, other):
        return Vec3(self.x - other.x, self.y - other.y, self.z - other.z)

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y and self.z == other.z

    def mul(self, mulp: float):
        return Vec3(self.x * mulp, self.y * mulp, self.z * mulp)

    def toVec2(self):
        self.z = 0

    def toJson(self):
        return {
            'x': self.x,
            'y': self.y,
            'z': self.z
        }
