#ifndef PLANE_H
#define PLANE_H

#include "object.h"

typedef struct plane {
    OBJECT_BASEFIELDS;

    vec3 pos;
    vec3 normal;
} plane;

plane *plane_create(vec3 pos, vec3 normal, material_t *material);

bool plane_hit(object_t *plane, ray_t *ray, double tmin, double tmax, hit_record *record);

#endif