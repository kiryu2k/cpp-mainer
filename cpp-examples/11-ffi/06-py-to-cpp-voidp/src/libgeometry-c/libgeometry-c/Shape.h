#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef void Shape;

double geometry_shape_area(const Shape* self);
void geometry_shape_delete(const Shape* self);

#ifdef __cplusplus
}
#endif
