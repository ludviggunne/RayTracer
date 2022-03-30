#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

typedef struct bmp_pixel {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} bmp_pixel;

typedef struct bmp {
    uint32_t width;
    uint32_t height;
    bmp_pixel *buffer;
} bmp;

void bmp_create(bmp *bitmap, uint32_t width, uint32_t height);

void bmp_set_pixel(bmp *bitmap, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);

void bmp_save(bmp *bitmap, const char *filename);

void bmp_free(bmp* bitmap);

void bmp_testimg(const char* output);

#endif