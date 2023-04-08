import ctypes


geometry_lib = ctypes.cdll.LoadLibrary('libgeometry-c.so')

class Point(ctypes.Structure):
    _fields_ = [
        ('x', ctypes.c_double),
        ('y', ctypes.c_double)
    ]

circle_new = geometry_lib.geometry_circle_new
circle_new.argtypes = [Point, ctypes.c_double]
circle_new.restype = ctypes.c_void_p

circle_radius = geometry_lib.geometry_circle_radius
circle_radius.argtypes = [ctypes.c_void_p]
circle_radius.restype = ctypes.c_double

shape_area = geometry_lib.geometry_shape_area
shape_area.argtypes = [ctypes.c_void_p]
shape_area.restype = ctypes.c_double

shape_delete = geometry_lib.geometry_shape_delete
shape_delete.argtypes = [ctypes.c_void_p]


class Shape:
    def __init__(self, obj):
        self._obj = obj

    def __del__(self):
        shape_delete(self._obj)

    def area(self):
        return shape_area(self._obj)


class Circle(Shape):
    def __init__(self, center: Point, radius: float):
        obj = circle_new(center, radius)
        super(Circle, self).__init__(obj)

    @property
    def radius(self):
        return circle_radius(self._obj)
