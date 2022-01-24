#include "dielectric.h"
#include "../utility.h"

#include <stdlib.h>

material_t* dielectric_create(double refr_id, double fuzz)
{
	dielectric* dlct = malloc(sizeof *dlct);
	if(dlct == NULL)
		ERROR(ERROR_ALLOC_FAIL, ERROR_DIELECTRIC, ERROR_NULL);

	*dlct = (dielectric){
		.scatter = &dielectric_scatter,
		.color = (vec3){1, 1, 1},

		.refr_id = refr_id,
		.fuzz = fuzz,
	};

	return (material_t*)dlct;
}

void dielectric_scatter(material_t* met, ray_t* scattered, hit_record* hrec, vec3* color, ray_t* ray, bool* final)
{

}