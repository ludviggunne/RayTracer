#ifndef UTILITY_H
#define UTILITY_H

#include "settings.h"
#include "structs/vector.h"
#include "image/bitmap.h"

#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define EPSILON 0.0000000001


/* ------------------------------------------ */

static unsigned int g_seed = 0;

// Used to seed the generator.           
inline void fast_srand(int seed) {
    g_seed = seed;
}

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
inline int fast_rand(void) {
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

#define FRAND_MAX 0b1000000000000000;

/* ------------------------------------------ */


inline double randd()
{
    return ((double)fast_rand()) / FRAND_MAX;
}

inline double randd_range(double min, double max)
{
    return min + randd() * (max - min);
}

inline vec3 randv()
{
    return (vec3) { randd(), randd(), randd() };
}

inline vec3 randv_range(double min, double max)
{
    return (vec3) { randd_range(min, max), randd_range(min, max), randd_range(min, max) };
}

inline void randuv(vec3* v)
{
    *v = (vec3){ randd_range(-1, 1), randd_range(-1, 1), randd_range(-1, 1) };
    vec3_nrm(v, v);
}

inline void gamma2(vec3* v)
{
    *v = (vec3){ sqrt(v->x), sqrt(v->y), sqrt(v->z) };
}

inline void atten(vec3* cout, vec3* catt)
{
    cout->x *= catt->x;
    cout->y *= catt->y;
    cout->z *= catt->z;
}

inline malloc_check(void* ptr)
{
    if (!ptr)
        printf("Malloc failed");
}

/* LOGGING */

#ifdef DEBUG
    #define DebugCall(expr) expr
    #define MALLOC_CHECK(ptr) malloc_check(ptr)
#else
    #define DebugCall(expr)
    #define MALLOC_CHECK(ptr)
#endif


/* ERRORS */
#define ERROR_NULL "\n"
#define ERROR_ALLOC_FAIL "Failed to allocate: "
#define ERROR_REALLOC_FAIL "Failed to reallocate: "

#define ERROR_SPHERE "Sphere"
#define ERROR_PLANE "Plane"
#define ERROR_MESH "Mesh"

#define ERROR_VB "Vertex buffer"
#define ERROR_IB "Index buffer"

#define ERROR_OBJ_LIST "Object list"
#define ERROR_MAT_LIST "Material list"

#define ERROR_DIFFUSE "Diffuse"
#define ERROR_METAL "Metal"
#define ERROR_DIELECTRIC "Dielectric"
#define ERROR_EMISSIVE "Emissive"

#define ERROR_DENOISED "Denoised image"

#define ERROR(e1, e2, e3) { DebugCall(printf(e1); printf(e2); printf(e3); exit(-1)); }

#endif