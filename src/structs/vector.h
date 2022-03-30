#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

typedef struct vec3 {
    double x;
    double y;
    double z;
} vec3;


void vec3_add(vec3* out, vec3* in1, vec3* in2);

void vec3_sub(vec3* out, vec3* in1, vec3* in2);


void vec3_mul(vec3* out, vec3* inv, double ins);

void vec3_div(vec3* out, vec3* inv, double ins);


void vec3_crs(vec3* out, vec3* in1, vec3* in2);

void vec3_dot(double* out, vec3* in1, vec3* in2);

void vec3_copy(vec3 *out, vec3 *in);



/* Functions */

double vec3_len2(vec3* vec);

double vec3_len(vec3* vec);

void vec3_nrm(vec3* out, vec3* in);

void vec3_zero(vec3* vec);

void vec3_refl(vec3* out, vec3* in, vec3* nrm);

#endif