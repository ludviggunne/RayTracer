#include "plane.h"

#include <stdlib.h>

#include "../utility.h"

plane *plane_create(vec3 pos, vec3 normal, material_t *material)
{
    plane *p = malloc(sizeof *p);

    if (p == NULL)
        ERROR(ERROR_ALLOC_FAIL, ERROR_PLANE, ERROR_NULL);

    vec3_nrm(&normal, &normal);
    *p = (plane){
        .hit = plane_hit,
        .material = material,
        .free_data = NULL,
        .pos = pos,
        .normal = normal
    };

    return p;
}

_Bool plane_hit(object_t *pl, ray_t *ray, double tmin, double tmax, hit_record *record)
{
    double denom;
    double nom;
    double t;
    vec3 vtmp;

    plane *p = (plane*)pl;
    vec3_dot(&denom, &ray->dir, &p->normal);

    /* Ray is parallell */
    if (denom == 0)
        return false;

    /* Calculate intersection */
    vtmp = p->pos;
    vec3_sub(&vtmp, &vtmp, &ray->org);
    vec3_dot(&nom, &vtmp, &p->normal);

    t = nom / denom;

    /* Intersection outside bounds */
    if (t < tmin || t > tmax)
        return false;


    /* Update record */
    record->t = t;
    hrec_set_normal(record, &p->normal, ray);
    ray_at(&record->point, ray, t);
    record->mat = p->material;

    return true;
}