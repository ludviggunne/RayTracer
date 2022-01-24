#include "ray.h"

void ray_at(vec3* out, ray_t* r, double t)
{
    vec3_mul(out, &r->dir, t);
    vec3_add(out, out, &r->org);
}