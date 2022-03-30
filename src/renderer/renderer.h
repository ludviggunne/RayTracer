#ifndef RENDERER_H
#define RENDERER_H

#include <stddef.h>

#include "../image/bitmap.h"
#include "../structs/ray.h"
#include "../structs/camera.h"
#include "../objects/object.h"


typedef struct renderer {
    uint32_t width;
    uint32_t height;

    bmp img;
    camera cam;

    object_t **objs;
    size_t num_obj;
    size_t obj_cap;

    size_t samples;
    size_t bounces;
    vec3 gradient_top;
    vec3 gradient_bottom;

} renderer;


void renderer_create(renderer *rdr, uint32_t width, uint32_t height, size_t num_obj_cap);

void renderer_add_sphere(renderer *rdr, vec3 pos, double radius, material_t *material);

void renderer_add_plane(renderer *rdr, vec3 pos, vec3 normal, material_t *material);

void renderer_add_mesh(renderer *rdr, vec3 pos, size_t vb_size, size_t ib_size, material_t *material);

void _renderer_objs_check_realloc(renderer* rdr);

void renderer_compute(renderer *rdr);

void _renderer_trace_ray(renderer *rdr, ray_t *ray, vec3 *color, size_t depth);

void renderer_saveimg(renderer *rdr, const char* filename);

void renderer_free(renderer *rdr);

#endif