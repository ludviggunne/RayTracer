#ifndef DIFFUSE_H
#define DIFFUSE_H

#include "material.h"

typedef struct diffuse {
    MATERIAL_BASEFIELDS;
} diffuse;

material_t *diffuse_create(vec3 color);

void diffuse_scatter(material_t *diff, ray_t *scattered, hit_record *hrec, vec3 *color, ray_t *ray, bool* final);

#endif