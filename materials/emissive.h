#ifndef EMISSIVE_H
#define EMISSIVE_H

#include "material.h"

typedef struct emissive {
	MATERIAL_BASEFIELDS;

	double strength;
} emissive;

material_t *emissive_create(vec3 color, double strength);

void emissive_scatter(material_t *emsv, ray_t *scattered, hit_record *hrec, vec3 *color, ray_t *ray, bool *final);


#endif