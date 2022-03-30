#include "vector.h"

void vec3_add(vec3* out, vec3* in1, vec3* in2)
{
    *out = (vec3){
        in1->x + in2->x,
        in1->y + in2->y,
        in1->z + in2->z
    };
}

void vec3_sub(vec3* out, vec3* in1, vec3* in2)
{
    *out = (vec3){
        in1->x - in2->x,
        in1->y - in2->y,
        in1->z - in2->z
    };
}


void vec3_mul(vec3* out, vec3* inv, double ins)
{
    *out = (vec3){
        inv->x * ins,
        inv->y * ins,
        inv->z * ins
    };
}

void vec3_div(vec3* out, vec3* inv, double ins)
{
    *out = (vec3){
        inv->x / ins,
        inv->y / ins,
        inv->z / ins
    };
}


void vec3_crs(vec3* out, vec3* in1, vec3* in2)
{
    vec3 out_ = (vec3){
        in1->y * in2->z - in1->z * in2->y,
        in1->z * in2->x - in1->x * in2->z,
        in1->x * in2->y - in1->y * in2->x
    };

    *out = out_;
}

void vec3_dot(double* out, vec3* in1, vec3* in2)
{
    *out = in1->x * in2->x + in1->y * in2->y + in1->z * in2->z;
}

void vec3_copy(vec3 *out, vec3 *in)
{
    *out = *in;
}



/* Functions */

double vec3_len2(vec3* vec)
{
    return vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
}

double vec3_len(vec3* vec)
{
    return sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}

void vec3_nrm(vec3* out, vec3* in)
{
    double len = vec3_len(in);
    
    *out = (vec3){
        in->x / len,
        in->y / len,
        in->z / len
    };
}

void vec3_zero(vec3* vec)
{
    *vec = (vec3){ 0, 0, 0 };
}

void vec3_refl(vec3* out, vec3* in, vec3* nrm)
{
    double dtmp;

    vec3_dot(&dtmp, in, nrm);
    vec3_mul(out, nrm, 2 * dtmp);
    vec3_sub(out, in, out);
}