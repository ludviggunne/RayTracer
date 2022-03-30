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

unsigned int g_seed;

// Used to seed the generator.           
void fast_srand(int seed);

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
int fast_rand(void);

#define FRAND_MAX 0b1000000000000000;

/* ------------------------------------------ */


double randd();

double randd_range(double min, double max);

vec3 randv();

vec3 randv_range(double min, double max);

void randuv(vec3* v);

void gamma2(vec3* v);

void atten(vec3* cout, vec3* catt);

void malloc_check(void* ptr);

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