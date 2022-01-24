#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "material.h"

typedef struct dielectric {
	MATERIAL_BASEFIELDS;
	double refr_id;
	double fuzz;
} dielectric;

material_t* dielectric_create(double refr_id, double fuzz);

void dielectric_scatter(material_t* met, ray_t* scattered, hit_record* hrec, vec3* color, ray_t* ray, bool* final);

#endif