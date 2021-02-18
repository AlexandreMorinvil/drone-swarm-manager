#vecteur 3d
class Vec3:
    x = 0
    y = 0
    z = 0
    def __init__(self, x,y,z) :
        self.x = x
        self.y = y
        self.z = z
    def __add__(self, other: Vec3) :
        return self.x + other.x, self.y + other.y, self.z + other.z

    def __sub__(self, other: Vec3) :
        return self.x - other.x, self.y - other.y, self.z - other.z

    def mul(self, mulp: float) :
        return self.x * mulp, self.y * mulp, self.z * mulp
    def toVec2(self) :
        self.z =0
