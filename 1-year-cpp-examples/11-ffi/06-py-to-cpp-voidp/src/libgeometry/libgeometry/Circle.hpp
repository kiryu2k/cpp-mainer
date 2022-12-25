#pragma once

#include <libgeometry/Point.hpp>
#include <libgeometry/Shape.hpp>

namespace geometry {

class Circle : public Shape {
 public:
  Circle(Point center, double radius) : center_(center), radius_(radius) {
  }

  double area() const override;
  double radius() const {
    return radius_;
  }

 private:
  Point center_;
  double radius_;
};

}  // namespace geometry
