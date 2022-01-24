#include "renderer/renderer.h"
#include "materials/mat_list.h"
#include "tests/test.h"
#include "utility.h"
#include "image/denoising.h"

#include "objects/mesh.h"
#include "objects/buffers.h"

#include <math.h>
#include <time.h>
#include <string.h>

#define SAMPLES 32
#define WIDTH 800
#define ASPECTR 1
#define HEIGHT ((uint32_t)(WIDTH / ASPECTR))


int main(int argc, char* argv[])
{
    renderer rdr;

    /* Renderer */
    renderer_create(&rdr, WIDTH, HEIGHT, 20);
    rdr.bounces = 8;
    rdr.samples = SAMPLES;


    /* Sky */
    rdr.gradient_bottom = (vec3) { .2, .1, .2 };
    rdr.gradient_top = (vec3) { .1, .1, .2 };


    /* Camera */
    camera cam;
    cam.focal_length = 1.5;
    cam.aperture = .05;
    cam.focus = (vec3) { .5, 1, .5 };
    cam.origin = (vec3) { 4, 3, 3 };
    cam.up = (vec3) { 0, 1, 0 };
    rdr.cam = cam;


    /* Materials */
    mat_list ml;
    mat_list_create(&ml, 20);

    mat_list_add_diffuse(&ml, (vec3) { 1, .5, 0 });
    mat_list_add_diffuse(&ml, (vec3) { .5, .5, .5 });
    mat_list_add_diffuse(&ml, (vec3) { .4, .3, 1 });
    mat_list_add_emissive(&ml, (vec3){ 1, 1, .9 }, 50);


    /* Objects */
    renderer_add_mesh(&rdr, (vec3) { 0, 0, 0 }, 8, 36, mat_list_at(&ml, 0));
    
    mesh_load_vertex_buffer((mesh*)rdr.objs[rdr.num_obj - 1], (vec3 *)vb_cube);
    mesh_load_index_buffer((mesh*)rdr.objs[rdr.num_obj - 1], ib_cube);

    
    renderer_add_sphere(&rdr, (vec3) { .5, 1.5, .5 }, .5, mat_list_at(&ml, 2));

    renderer_add_sphere(&rdr, (vec3) { 5, 5, -3 }, 2, mat_list_at(&ml, 3));

    renderer_add_plane(&rdr, (vec3) { 0, 0, 0 }, (vec3) { 0, 1, 0 }, mat_list_at(&ml, 1));


    /* Render */
    clock_t start = clock();
    renderer_compute(&rdr);
    clock_t diff = clock() - start;

    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Render time: %ds %dms\n", msec / 1000, msec % 1000);

    renderer_saveimg(&rdr, "output.bmp");


    /* Cleanup */
    renderer_free(&rdr);
    mat_list_free(&ml);

    return 0;
}