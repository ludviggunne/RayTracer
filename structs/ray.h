#ifndef RAY_H
#define RAY_H

#include "vector.h"

typedef struct ray_t {
    vec3 org;
    vec3 dir;
} ray_t;

void ray_at(vec3* out, ray_t* r, double t);

#endif