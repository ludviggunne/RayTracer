#ifndef MESH_H
#define MESH_H

#include "object.h"
#include <stdint.h>

typedef struct mesh {
	OBJECT_BASEFIELDS;

	vec3 *vertex_buffer;
	size_t vertex_buffer_size;

	uint32_t *index_buffer;
	size_t index_buffer_size;
} mesh;

object_t *mesh_create(size_t vertex_buffer_size, size_t index_buffer_size, material_t *material);

bool mesh_hit(object_t *m, ray_t *ray, double tmin, double tmax, hit_record *record);

void mesh_load_vertex_buffer(mesh *m, vec3 *buffer);

void mesh_load_index_buffer(mesh *m, uint32_t *buffer);

void mesh_rotate(mesh *m, double u, double v, double w);

void mesh_translate(mesh *m, vec3 pos);

void mesh_scale(mesh *m, double sx, double sy, double sz);

void mesh_free(object_t *m);

#endif