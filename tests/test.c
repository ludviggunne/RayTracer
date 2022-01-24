#include "test.h"
#include "../renderer/renderer.h"
#include "../materials/diffuse.h"
#include "../materials/metal.h"
#include "../utility.h"
#include "../objects/sphere.h"
#include "../materials/emissive.h"

void gen_test(void)
{
	const size_t nspheres = 200;
	const double rmin = .05;
	const double rmax = .3;
	const double rspawn = 1;

	/* Create renderer */
	renderer rdr;
	renderer_create(&rdr, 800, 600, nspheres);
	rdr.bounces = 8;
	rdr.samples = 4;

	rdr.gradient_bottom = (vec3){ .8, .9, .5 };
	rdr.gradient_top = (vec3){ .1, .1, .5 };
	

	/* Setup camera */
	rdr.cam.focal_length = 1.5;
	rdr.cam.focus = (vec3){ 0, 0, 0 };
	rdr.cam.origin = (vec3){ -4, 3, 0 };
	rdr.cam.up = (vec3){ 0, 1, 0 };


	/* Setup material */
	material_t* mat = metal_create((vec3) { .8, .8, .8 }, .4);


	/* Add spheres */
	size_t n = 0;
	while (n < nspheres)
	{
		vec3 pos = randv_range(-rspawn, rspawn);
		if (vec3_len(&pos) > rspawn)
			continue;

		double r = randd_range(rmin, rmax);
		bool coll = false;
		for (size_t i = 0; i < n; i++)
		{
			sphere *s = (sphere*)rdr.objs[i];
			vec3 p = s->pos;
			vec3_sub(&p, &p, &pos);

			if (vec3_len(&p) < s->radius + r)
			{
				coll = true;
				break;
			}
		}

		if (coll)
			continue;

		renderer_add_sphere(&rdr, pos, r, mat);
		n++;
	}

	renderer_compute(&rdr);
	renderer_saveimg(&rdr, "test.bmp");
	renderer_free(&rdr);
}

void emsv_test(void)
{

	/* Create renderer */
	renderer rdr;
	renderer_create(&rdr, 800, 600, 2);
	rdr.bounces = 8;
	rdr.samples = 512;

	rdr.gradient_bottom = (vec3){ .1, .1, 0 };
	rdr.gradient_top = (vec3){ .05, 0, .15 };


	/* Setup camera */
	rdr.cam.focal_length = 1;
	rdr.cam.focus = (vec3){ 0, 0, 0 };
	rdr.cam.origin = (vec3){ -4, 3, 0 };
	rdr.cam.up = (vec3){ 0, 1, 0 };


	/* Setup material */
	material_t* diff = diffuse_create((vec3) { .7, .3, .4 });
	material_t* emsv = emissive_create((vec3) { 1, 1, .8 }, 10);

	renderer_add_sphere(&rdr, (vec3) { 0, 0, 0 }, .5, diff);
	renderer_add_sphere(&rdr, (vec3) { 0, 2, 0 }, .5, emsv);

	renderer_compute(&rdr);
	renderer_saveimg(&rdr, "test.bmp");
	renderer_free(&rdr);
}