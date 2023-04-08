#pragma once

#include <libgeometry-c/Point.h>
#include <libgeometry-c/Shape.h>

#ifdef __cplusplus
extern "C" {
#endif

Shape* geometry_circle_new(Point center, double radius);
double geometry_circle_radius(const Shape* self);

#ifdef __cplusplus
}
#endif
