#ifndef MATERIAL_H
#define MATERIAL_H

#include "../structs/ray.h"
#include "../structs/hit_record.h"

#include <stdbool.h>

#define MATERIAL_BASEFIELDS \
    void (*scatter)(material_t*, ray_t*, hit_record*, vec3*, ray_t*, bool*);\
    vec3 color

typedef struct material_t {
    MATERIAL_BASEFIELDS;
} material_t;

#endif