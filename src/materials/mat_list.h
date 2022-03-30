#ifndef MAT_LIST_H
#define MAT_LIST_H

#include "material.h"

#include <stdlib.h>

typedef struct mat_list {
	material_t **mats;
	size_t size;
	size_t cap;
} mat_list;

void mat_list_create(mat_list *ml, size_t capacity);

void mat_list_add_diffuse(mat_list *ml, vec3 color);

void mat_list_add_metal(mat_list *ml, vec3 color, double diff);

void mat_list_add_emissive(mat_list* ml, vec3 color, double strength);

material_t *mat_list_at(mat_list *ml, size_t i);

void _mat_list_check_realloc(mat_list *ml);

void mat_list_free(mat_list *ml);

#endif