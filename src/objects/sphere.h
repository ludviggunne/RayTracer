#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

typedef struct sphere {
    OBJECT_BASEFIELDS;

    vec3 pos;
    double radius;
} sphere;

sphere *sphere_create(vec3 pos, double radius, material_t *material);

bool sphere_hit(object_t *sph, ray_t *ray, double tmin, double tmax, hit_record *record);

#endif
