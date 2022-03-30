#include "sphere.h"

#include <math.h>
#include <stdlib.h>

#include "../utility.h"

sphere *sphere_create(vec3 pos, double radius, material_t *material)
{
    sphere *s = malloc(sizeof *s);

    if (s == NULL)
        ERROR(ERROR_ALLOC_FAIL, ERROR_SPHERE, ERROR_NULL);

    *s = (sphere){
        .hit = sphere_hit,
        .material = material,
        .free_data = NULL,
        .pos = pos,
        .radius = radius
    };

    return s;
}

bool sphere_hit(object_t *sph, ray_t *ray, double tmin, double tmax, hit_record *record)
{
    double a;
    double b;
    double c;
    double ctmp;
    double discr;

    double t0;
    double t1;
    double ttmp;
    vec3 vtmp;

    sphere *s = (sphere*)sph;


    /* A */
    vec3_dot(&a, &ray->dir, &ray->dir);

    /* B */
    vtmp = ray->org;
    vec3_sub(&vtmp, &vtmp, &s->pos);
    vec3_dot(&b, &vtmp, &ray->dir);
    b *= 2;

    /* C */
    vtmp = ray->org;
    vec3_mul(&vtmp, &vtmp, 2);
    vec3_sub(&vtmp, &s->pos, &vtmp);
    vec3_dot(&c, &s->pos, &vtmp);
    vec3_dot(&ctmp, &ray->org, &ray->org);
    c += ctmp;
    c -= s->radius * s->radius;


    discr = b * b - 4 * a * c;

    /* No Solution */
    if(discr < 0)
        return false;

    t0 = (-b + sqrt(discr)) / 2 / a;
    t1 = (-b - sqrt(discr)) / 2 / a;

    /* Order t0 and t1 */
    if(t1 < t0)
    {
        ttmp = t0;
        t0 = t1;
        t1 = t0;
    }


    /* Check for closest solution in range */
    if(t0 < tmin || t0 > tmax)
    {
        t0 = t1;

        if(t0 < tmin || t0 > tmax)
            return false;
    }


    /* Update hit record */
    record->t = t0;
    ray_at(&record->point, ray, t0);

    vec3_sub(&vtmp, &record->point, &s->pos);
    vec3_nrm(&vtmp, &vtmp);

    hrec_set_normal(record, &vtmp, ray);

    record->mat = s->material;

    return true;
}
