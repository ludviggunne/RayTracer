#include "renderer.h"

#include "../utility.h"

void renderer_compute(renderer *rdr)
{
#ifdef SHOW_NORMALS_ONLY
    rdr->cam.aperture = 0;
#endif

    /* Calculate viewport */

    double aspectr = ((double)rdr->height) / rdr->width;
    double focus_dist;

    vec3 cam_dir;
    vec3_sub(&cam_dir, &rdr->cam.focus, &rdr->cam.origin);
    focus_dist = vec3_len(&cam_dir);
    vec3_nrm(&cam_dir, &cam_dir);

    /* Horizontal unit vector */
    vec3 viewport_unit_u;
    vec3_crs(&viewport_unit_u, &rdr->cam.up, &cam_dir);
    vec3_nrm(&viewport_unit_u, &viewport_unit_u);
    vec3_mul(&viewport_unit_u, &viewport_unit_u, focus_dist / rdr->cam.focal_length);

    /* Vertical unit vector */
    vec3 viewport_unit_v;
    vec3_crs(&viewport_unit_v, &cam_dir, &viewport_unit_u);
    vec3_nrm(&viewport_unit_v, &viewport_unit_v);
    vec3_mul(&viewport_unit_v, &viewport_unit_v, focus_dist / rdr->cam.focal_length);

    vec3_mul(&viewport_unit_v, &viewport_unit_v, aspectr);

    /* Viewport corner */
    vec3_mul(&cam_dir, &cam_dir, rdr->cam.focal_length);

    vec3 viewport_corner = (vec3){ 0, 0, 0 };
    vec3_sub(&viewport_corner, &viewport_corner, &viewport_unit_u);
    vec3_sub(&viewport_corner, &viewport_corner, &viewport_unit_v);
    vec3_div(&viewport_corner, &viewport_corner, 2);
    vec3_add(&viewport_corner, &viewport_corner, &rdr->cam.focus);

    /* Defocus unit vectors */
    vec3 defocus_unit_u = viewport_unit_u;
    vec3 defocus_unit_v = viewport_unit_v;
    vec3_nrm(&defocus_unit_u, &defocus_unit_u);
    vec3_nrm(&defocus_unit_v, &defocus_unit_v);


    /* Iterate */
    const uint32_t step = rdr->height / 100;
    for (uint32_t y = 0; y < rdr->height; y++)
    {
        DebugCall(
            if(y % step == 0)
                printf("%d%%\n", (uint32_t)(100 * y / (double)(rdr->height)))
        );
        for (uint32_t x = 0; x < rdr->width; x++)
        {
            vec3 color = (vec3){ 0, 0, 0 };

            for (size_t sample_i = 0; sample_i < rdr->samples; sample_i++)
            {
                /* Calculate ray */
                vec3 viewport_pos_u = viewport_unit_u;
                vec3 viewport_pos_v = viewport_unit_v;

                vec3_mul(&viewport_pos_u, &viewport_pos_u, (x + randd()) / rdr->width);
                vec3_mul(&viewport_pos_v, &viewport_pos_v, (y + randd()) / rdr->height);

                ray_t ray = (ray_t){
                    .org = rdr->cam.origin,
                    .dir = viewport_corner

                };

                vec3_add(&ray.dir, &ray.dir, &viewport_pos_u);
                vec3_add(&ray.dir, &ray.dir, &viewport_pos_v);

                /* Defocus */
                vec3 defocus_u;
                vec3 defocus_v;
                vec3 defocus = (vec3){ 10, 10, 10 };

                while (vec3_len(&defocus) > 1)
                {
                    defocus_u = defocus_unit_u;
                    defocus_v = defocus_unit_v;

                    vec3_mul(&defocus_u, &defocus_u, randd_range(-1, 1));
                    vec3_mul(&defocus_v, &defocus_v, randd_range(-1, 1));

                    vec3_add(&defocus, &defocus_u, &defocus_v);
                }
                vec3_mul(&defocus, &defocus, rdr->cam.aperture);
                vec3_add(&ray.org, &ray.org, &defocus);
                vec3_sub(&ray.dir, &ray.dir, &ray.org);


                /* Trace ray */
                vec3 sample_color = (vec3){ 1, 1, 1 };

                _renderer_trace_ray(rdr, &ray, &sample_color, rdr->bounces);

                vec3_add(&color, &color, &sample_color);
            }


            /* Write pixel data */
            vec3_div(&color, &color, (double)rdr->samples);
            gamma2(&color);

            rdr->img.buffer[x + rdr->width * y] = (bmp_pixel){
                (uint8_t)(255.999 * color.z),
                (uint8_t)(255.999 * color.y),
                (uint8_t)(255.999 * color.x)
            };
        }
    }
}