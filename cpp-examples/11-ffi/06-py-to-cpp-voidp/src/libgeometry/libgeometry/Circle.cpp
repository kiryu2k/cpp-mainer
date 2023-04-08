#include <libgeometry/Circle.hpp>

#include <numbers>

namespace geometry {

double Circle::area() const {
  return std::numbers::pi * radius_ * radius_;
}

}  // namespace geometry
