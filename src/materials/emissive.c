#include "emissive.h"
#include "../utility.h"

material_t* emissive_create(vec3 color, double strength)
{
	emissive* em = malloc(sizeof *em);
	if (em == NULL)
		ERROR(ERROR_ALLOC_FAIL, ERROR_EMISSIVE, ERROR_NULL);

	*em = (emissive){
		.scatter = &emissive_scatter,
		.color = color,

		.strength = strength
	};

	return (material_t*)em;
}

void emissive_scatter(material_t* emsv, ray_t* scattered, hit_record* hrec, vec3* color, ray_t* ray, bool *final)
{
	/* No scatter from emissive material */
	*final = true;

	vec3 c = *color;
	c.x = pow(c.x, 1 / ((emissive*)emsv)->strength);
	c.y = pow(c.x, 1 / ((emissive*)emsv)->strength);
	c.z = pow(c.x, 1 / ((emissive*)emsv)->strength);

	atten(&c, &emsv->color);
	*color = c;
}
