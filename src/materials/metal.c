#include "metal.h"
#include "../utility.h"

#include <stdlib.h>

material_t *metal_create(vec3 color, double fuzz)
{
    metal* met = malloc(sizeof *met);
    if (met == NULL)
        ERROR(ERROR_ALLOC_FAIL, ERROR_METAL, ERROR_NULL);

    *met = (metal){
        .scatter = &metal_scatter,
        .color = color,

        .fuzz = fuzz
    };

    return (material_t*)met;
}

void metal_scatter(material_t* met, ray_t* scattered, hit_record* hrec, vec3* color, ray_t* ray, bool *final)
{
    /* Scatter */
    metal* m = (metal*)met;
    scattered->org = hrec->point;

    vec3 rv;
    vec3 vtmp;
    vec3_refl(&vtmp, &ray->dir, &hrec->normal);
    randuv(&rv);
    vec3_mul(&rv, &rv, m->fuzz);
    vec3_add(&scattered->dir, &vtmp, &rv);

    /* Attenuate */
    atten(color, &met->color);
}