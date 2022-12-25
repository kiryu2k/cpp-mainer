import geometry


circle = geometry.circle_new(geometry.Point(0, 0), 2)
print(geometry.shape_area(circle))

c = geometry.Circle(geometry.Point(0, 0), 2)
print(c.area())
print(c.radius)
