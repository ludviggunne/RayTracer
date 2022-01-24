#include "mat_list.h"
#include "diffuse.h"
#include "metal.h"
#include "emissive.h"

#include <string.h>

#include "../utility.h"

void mat_list_create(mat_list *ml, size_t capacity)
{
	ml->size = 0;
	ml->cap = capacity;
	ml->mats = malloc(ml->cap * sizeof *ml->mats);

	if (ml->mats)
		ERROR(ERROR_ALLOC_FAIL, ERROR_MAT_LIST, ERROR_NULL);
}

void mat_list_add_diffuse(mat_list* ml, vec3 color)
{
	_mat_list_check_realloc(ml);

	ml->mats[ml->size++] = diffuse_create(color);
}

void mat_list_add_metal(mat_list* ml, vec3 color, double diff)
{
	_mat_list_check_realloc(ml);

	ml->mats[ml->size++] = metal_create(color, diff);
}

void mat_list_add_emissive(mat_list* ml, vec3 color, double strength)
{
	_mat_list_check_realloc(ml);

	ml->mats[ml->size++] = emissive_create(color, strength);
}

void _mat_list_check_realloc(mat_list* ml)
{
	material_t **new_mats;
	if (ml->size < ml->cap)
		return;

	ml->cap *= 2;
	new_mats = malloc(ml->cap * sizeof *new_mats);

	if(new_mats == NULL)
		ERROR(ERROR_ALLOC_FAIL, ERROR_MAT_LIST, ERROR_NULL);

	memcpy(new_mats, ml->mats, ml->size * sizeof *ml->mats);

	free(ml->mats);
	ml->mats = new_mats;
}

material_t *mat_list_at(mat_list *ml, size_t i)
{
	return ml->mats[i];
}

void mat_list_free(mat_list *ml)
{
	free(ml->mats);
}