#include "diffuse.h"
#include "../utility.h"

#include <stdlib.h>

material_t *diffuse_create(vec3 color)
{
    diffuse *diff = malloc(sizeof *diff);
    if (diff == NULL)
        ERROR(ERROR_ALLOC_FAIL, ERROR_DIFFUSE, ERROR_NULL);

    *diff = (diffuse){
        .scatter = &diffuse_scatter,
        .color = color
    };

    return (material_t*)diff;
}

void diffuse_scatter(material_t *diff, ray_t *scattered, hit_record *hrec, vec3 *color, ray_t *ray, bool *final)
{

    scattered->org = hrec->point;

    /* Scatter */
    scattered->dir = hrec->normal;

    vec3 rv;
    randuv(&rv);
    vec3_add(&scattered->dir, &scattered->dir, &rv);

    /* Attenuate */
    atten(color, &diff->color);
}