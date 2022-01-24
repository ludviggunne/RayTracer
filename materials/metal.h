#ifndef METAL_H
#define METAL_H

#include "material.h"

typedef struct metal {
    MATERIAL_BASEFIELDS;

    double fuzz;
} metal;

material_t *metal_create(vec3 color, double fuzz);

void metal_scatter(material_t* met, ray_t* scattered, hit_record* hrec, vec3* color, ray_t* ray, bool *final);

#endif