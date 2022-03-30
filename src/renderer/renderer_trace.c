#include "renderer.h"

#include "../utility.h"

void _renderer_trace_ray(renderer *rdr, ray_t *ray, vec3 *color, size_t depth)
{
    /* Max bounces reached */
    if (depth == 0)
    {
        *color = (vec3){ 0, 0, 0 };
        return;
    }


    /* Determine closest hit object */
    double t_nearest = DBL_MAX;
    bool hit = false;

    hit_record hrec;
    hit_record tmp_hrec;

    for (size_t obj_i = 0; obj_i < rdr->num_obj; obj_i++)
    {
        object_t *obj = rdr->objs[obj_i];
        if (
            obj->hit(obj, ray, 0.00001, t_nearest, &tmp_hrec)       /* Hit */
            && tmp_hrec.t < t_nearest)                              /* Closest */
        {
            hrec = tmp_hrec;
            t_nearest = hrec.t;
            hit = true;
        }
    }

    /* Scatter / Diverge */
    if (hit)
    {
        /* Scatter using hit record info */
        ray_t scattered;
        bool final = false;
        hrec.mat->scatter(hrec.mat, &scattered, &hrec, color, ray, &final);

#ifdef SHOW_NORMALS_ONLY
        vec3 s = (vec3){ .5, .5, .5 };
        vec3_nrm(&hrec.normal, &hrec.normal);
        if (hrec.back_face)
            atten(&hrec.normal, &s);
        *color = hrec.normal;
        return;
#endif

        /* Trace scattered ray */
        if (!final)
            _renderer_trace_ray(rdr, &scattered, color, depth - 1);
    }
    else /* No hit -> sky color*/
    {
        /* Find sky gradient */
        vec3_nrm(&ray->dir, &ray->dir);
        double grad_t = .5 * (ray->dir.y + 1);

        /* Interpolate */
        vec3 sky_col = rdr->gradient_top;
        vec3_sub(&sky_col, &sky_col, &rdr->gradient_bottom);
        vec3_mul(&sky_col, &sky_col, grad_t);
        vec3_add(&sky_col, &sky_col, &rdr->gradient_bottom);

        atten(color, &sky_col);
    }
}