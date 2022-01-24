#include "hit_record.h"

void hrec_set_normal(hit_record* hrec, vec3* normal, ray_t* ray)
{
	double dot;
	vec3 norm = *normal;
	vec3_dot(&dot, normal, &ray->dir);
	hrec->back_face = dot > 0;
	if (hrec->back_face)
		vec3_mul(&norm, &norm, -1);

	hrec->normal = norm;
}