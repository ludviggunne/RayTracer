#include "renderer.h"

#include "../objects/sphere.h"
#include "../objects/plane.h"
#include "../objects/mesh.h"
#include "../utility.h"

#include <stdlib.h>
#include <string.h>

void renderer_create(renderer *rdr, uint32_t width, uint32_t height, size_t num_obj_cap)
{
    rdr->width = width;
    rdr->height = height;


    /* Init image */
    bmp_create(&rdr->img, width, height);


    /* Init objects */
    rdr->num_obj = 0;
    rdr->obj_cap = num_obj_cap;
    rdr->objs = malloc(num_obj_cap * sizeof * rdr->objs);
    if (rdr->objs == NULL)
        ERROR(ERROR_ALLOC_FAIL, ERROR_OBJ_LIST, ERROR_NULL);


    /* Defaults */
    rdr->cam = (camera){
        .focal_length = 1.0,
        .aperture = 0,
        .origin = (vec3) { 0, 0, 0 },
        .focus = (vec3) { 0, 0, 1  },
        .up = (vec3) { 0, 1, 0  } };
    rdr->bounces = 8;
    rdr->samples = 1;
    rdr->gradient_top = (vec3){ .7, .7, 1 };
    rdr->gradient_bottom = (vec3){ 1, 1, .7 };
}

void renderer_add_sphere(renderer *rdr, vec3 pos, double radius, material_t *material)
{
    _renderer_objs_check_realloc(rdr);

    rdr->objs[rdr->num_obj++] = (object_t *)sphere_create(pos, radius, material);
}

void renderer_add_plane(renderer *rdr, vec3 pos, vec3 normal, material_t *material)
{
    _renderer_objs_check_realloc(rdr);

    rdr->objs[rdr->num_obj++] = (object_t *)plane_create(pos, normal, material);
}

void renderer_add_mesh(renderer *rdr, vec3 pos, size_t vb_size, size_t ib_size, material_t *material)
{
    _renderer_objs_check_realloc(rdr);

    rdr->objs[rdr->num_obj++] = (object_t *)mesh_create(vb_size, ib_size, material);
}

void _renderer_objs_check_realloc(renderer *rdr)
{
    object_t **new_objs;
    if (rdr->num_obj < rdr->obj_cap)
        return;

    rdr->obj_cap *= 2;

    new_objs = malloc(rdr->obj_cap * sizeof * new_objs);
    if (new_objs == NULL)
        ERROR(ERROR_REALLOC_FAIL, ERROR_OBJ_LIST, ERROR_NULL);

    memcpy(new_objs, rdr->objs, rdr->num_obj * sizeof * rdr->objs);

    free(rdr->objs);
    rdr->objs = new_objs;
}

void renderer_saveimg(renderer *rdr, const char *filename)
{
    bmp_save(&rdr->img, filename);
}

void renderer_free(renderer *rdr)
{
    for (size_t i = 0; i < rdr->num_obj; i++)
    {
        object_t *obj = rdr->objs[i];

        if (obj->free_data != NULL)
            obj->free_data(obj);

        free(rdr->objs[i]);
    }

    free(rdr->objs);
    bmp_free(&rdr->img);
}
