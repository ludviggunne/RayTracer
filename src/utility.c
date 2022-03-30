
#include "utility.h"

unsigned int g_seed = 0;

// Used to seed the generator.           
void fast_srand(int seed) {
    g_seed = seed;
}

int fast_rand(void) {
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}


double randd()
{
    return ((double)fast_rand()) / FRAND_MAX;
}

double randd_range(double min, double max)
{
    return min + randd() * (max - min);
}

vec3 randv()
{
    return (vec3) { randd(), randd(), randd() };
}

vec3 randv_range(double min, double max)
{
    return (vec3) { randd_range(min, max), randd_range(min, max), randd_range(min, max) };
}

void randuv(vec3* v)
{
    *v = (vec3){ randd_range(-1, 1), randd_range(-1, 1), randd_range(-1, 1) };
    vec3_nrm(v, v);
}

void gamma2(vec3* v)
{
    *v = (vec3){ sqrt(v->x), sqrt(v->y), sqrt(v->z) };
}

void atten(vec3* cout, vec3* catt)
{
    cout->x *= catt->x;
    cout->y *= catt->y;
    cout->z *= catt->z;
}

void malloc_check(void* ptr)
{
    if (!ptr)
        printf("Malloc failed");
}