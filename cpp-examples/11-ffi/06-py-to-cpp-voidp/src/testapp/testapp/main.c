#include <libgeometry-c/geometry.h>

#include <stdio.h>

int main() {
  const Point pt = {0, 0};
  const Shape* circle = geometry_circle_new(pt, 2);
  const double area = geometry_shape_area(circle);
  printf("circle area = %.6lf\n", area);
  const double radius = geometry_circle_radius(circle);
  printf("circle radius = %.6lf\n", radius);
  geometry_shape_delete(circle);
}
