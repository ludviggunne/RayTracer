#ifndef OBJECT_H
#define OBJECT_H

#include <stdbool.h>
#include "../materials/material.h"

typedef struct object_t object_t;

#define OBJECT_BASEFIELDS \
    bool (*hit)(object_t*, ray_t*, double, double, hit_record*);\
    material_t *material; \
    void (*free_data)(object_t*)

typedef struct object_t {
    OBJECT_BASEFIELDS;
} object_t;

#endif