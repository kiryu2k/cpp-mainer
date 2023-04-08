#include <libgeometry-c/Shape.h>

#include <libgeometry/Shape.hpp>

double geometry_shape_area(const Shape* self) {
  const auto* shape = static_cast<const geometry::Shape*>(self);
  return shape->area();
}

void geometry_shape_delete(const Shape* self) {
  const auto* shape = static_cast<const geometry::Shape*>(self);
  delete shape;
}
