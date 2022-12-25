#include <libgeometry-c/Circle.h>

#include <libgeometry/Circle.hpp>

Shape* geometry_circle_new(Point center, double radius) {
  const geometry::Point pt{center.x_, center.y_};
  return reinterpret_cast<Shape*>(
      static_cast<geometry::Shape*>(new geometry::Circle(pt, radius)));
}

double geometry_circle_radius(const Shape* self) {
  const auto* shape = reinterpret_cast<const geometry::Shape*>(self);
  const auto* circle = dynamic_cast<const geometry::Circle*>(shape);
  return circle->radius();
}
