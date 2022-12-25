#pragma once

namespace geometry {

class Shape {
 public:
  virtual ~Shape() = default;
  virtual double area() const = 0;
};

}  // namespace geometry
