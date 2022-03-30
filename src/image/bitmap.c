#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>

#include "../utility.h"


void bmp_create(bmp *bitmap, uint32_t width, uint32_t height)
{
    bitmap->width = width;
    bitmap->height = height;
    bitmap->buffer = malloc(width * height * sizeof *bitmap->buffer);

    MALLOC_CHECK(bitmap->buffer);
}

void bmp_set_pixel(bmp *bitmap, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
    bmp_pixel *pixel = bitmap->buffer + bitmap->width * y + x;
    *pixel = (bmp_pixel) { b, g, r }; 
}

void bmp_save(bmp *bitmap, const char* filename)
{
    /*
        Reference: https://cdn.hackaday.io/files/274271173436768/Simplified%20Windows%20BMP%20Bitmap%20File%20Format%20Specification.htm
    */


    /* Calculate padding & size */
    uint32_t padded_width = bitmap->width;
    uint32_t padding = 0;
    while(padded_width & 3)
    {
        padded_width++;
        padding++;
    }

    uint32_t size = 54 + 3 * bitmap->height * padded_width;

    
    FILE *f;
    fopen_s(&f, filename, "w+b");



    /* File header */

    if (f) {
        char buffer[8];


        fwrite("BM", 1, 2, f); /* bfType */

        *(uint32_t*)buffer = size;
        fwrite(buffer, 4, 1, f); /* bfSize */

        *(uint32_t*)buffer = 0;
        fwrite(buffer, 4, 1, f); /* bfReserved1 & 2 */

        *(uint32_t*)buffer = 54;
        fwrite(buffer, 4, 1, f); /* bfOffBits */



        /* Image header */

        *(uint32_t*)buffer = 40;
        fwrite(buffer, 4, 1, f); /* biSize */

        *(uint32_t*)buffer = bitmap->width;
        fwrite(buffer, 4, 1, f); /* biWidth */

        *(uint32_t*)buffer = bitmap->height;
        fwrite(buffer, 4, 1, f); /* biHeight */

        *(uint16_t*)buffer = 1;
        fwrite(buffer, 2, 1, f); /* biPlanes */

        *(uint16_t*)buffer = 24;
        fwrite(buffer, 2, 1, f); /* biBitCount */

        *(uint64_t*)buffer = 0;
        fwrite(buffer, 8, 1, f); /* biCompression & biSizeImage */
        fwrite(buffer, 8, 1, f); /* biXPelsPerMeter & biYPelsPerMeter */
        fwrite(buffer, 8, 1, f); /* biClrused & biClrImportant */



        /* Pixel data */
        *(uint8_t*)buffer = 0;
        for (uint32_t row = 0; row < bitmap->height; row++)
        {
            fwrite(
                bitmap->buffer + row * bitmap->width,
                3, bitmap->width, f);

            fwrite(buffer, padding, 1, f);
        }

        fclose(f);
    }
    else
        printf("Couldn't open/create file");
}

void bmp_free(bmp *bitmap)
{
    free(bitmap->buffer);
}

void bmp_testimg(const char* output)
{
    bmp img;
    bmp_create(&img, 256, 256);

    for (uint32_t x = 0; x < 256; x++)
        for (uint32_t y = 0; y < 256; y++)
            bmp_set_pixel(&img, x, y, (uint32_t)x, 0, (uint32_t)y);

    bmp_save(&img, output);

    bmp_free(&img);
}
