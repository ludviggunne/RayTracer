#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

typedef struct camera {
    double focal_length;
    double aperture;

    vec3 origin;
    vec3 focus;
    vec3 up;
} camera;

#endif