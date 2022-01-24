#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "vector.h"
#include "ray.h"

#include <stdbool.h>

typedef struct material_t material_t;

typedef struct hit_record {
    vec3 point;
    vec3 normal;
    double t;
    material_t *mat;
    bool back_face;
} hit_record;

void hrec_set_normal(hit_record *hrec, vec3 *normal, ray_t *ray);

#endif