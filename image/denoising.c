#include "denoising.h"
#include "../utility.h"

#include <math.h>
#include <stdlib.h>

void denoise(bmp* img)
{
	uint32_t new_w = img->width - 2;
	uint32_t new_h = img->height - 2;
	
	bmp_pixel* buffer0 = img->buffer;
	bmp_pixel* buffer1 = malloc(new_w * new_h * sizeof *buffer1);

	if(buffer1 == NULL)
		ERROR(ERROR_ALLOC_FAIL, ERROR_DENOISED, ERROR_NULL);

	for(uint32_t x = 0; x < new_w; x++)
		for (uint32_t y = 0; y < new_h; y++)
		{
			/* Accumulators */
			int32_t rhacc =
				buffer0[x + img->width * y].r +
				buffer0[x + img->width * (y + 1)].r +
				buffer0[x + img->width * (y + 2)].r -
				buffer0[x + 2 + img->width * y].r -
				buffer0[x + 2 + img->width * (y + 1)].r -
				buffer0[x + 2 + img->width * (y + 2)].r;
			int32_t ghacc =
				buffer0[x + img->width * y].g +
				buffer0[x + img->width * (y + 1)].g +
				buffer0[x + img->width * (y + 2)].g -
				buffer0[x + 2 + img->width * y].g -
				buffer0[x + 2 + img->width * (y + 1)].g -
				buffer0[x + 2 + img->width * (y + 2)].g;
			int32_t bhacc =
				buffer0[x + img->width * y].b +
				buffer0[x + img->width * (y + 1)].b +
				buffer0[x + img->width * (y + 2)].b -
				buffer0[x + 2 + img->width * y].b -
				buffer0[x + 2 + img->width * (y + 1)].b -
				buffer0[x + 2 + img->width * (y + 2)].b;

			int32_t rvacc =
				buffer0[x + img->width * y].r +
				buffer0[x + 1 + img->width * y].r +
				buffer0[x + 2 + img->width * y].r -
				buffer0[x + img->width * (y + 2)].r -
				buffer0[x + 1 + img->width * (y + 2)].r -
				buffer0[x + 2 + img->width * (y + 2)].r;
			int32_t gvacc =
				buffer0[x + img->width * y].g +
				buffer0[x + 1 + img->width * y].g +
				buffer0[x + 2 + img->width * y].g -
				buffer0[x + img->width * (y + 2)].g -
				buffer0[x + 1 + img->width * (y + 2)].g -
				buffer0[x + 2 + img->width * (y + 2)].g;
			int32_t bvacc =
				buffer0[x + img->width * y].b +
				buffer0[x + 1 + img->width * y].b +
				buffer0[x + 2 + img->width * y].b -
				buffer0[x + img->width * (y + 2)].b -
				buffer0[x + 1 + img->width * (y + 2)].b -
				buffer0[x + 2 + img->width * (y + 2)].b;

			uint32_t rgacc =
				buffer0[x + img->width * y].r +
				2 * buffer0[x + 1 + img->width * y].r +
				buffer0[x + 2 + img->width * y].r +
				2 *buffer0[x + img->width * (y + 1)].r +
				4 * buffer0[x + 1 + img->width * (y + 1)].r +
				2 * buffer0[x + 2 + img->width * (y + 1)].r +
				buffer0[x + img->width * (y + 2)].r +
				2 * buffer0[x + 1 + img->width * (y + 2)].r +
				buffer0[x + 2 + img->width * (y + 2)].r;
			uint32_t ggacc =
				buffer0[x + img->width * y].g +
				2 * buffer0[x + 1 + img->width * y].g +
				buffer0[x + 2 + img->width * y].g +
				2 * buffer0[x + img->width * (y + 1)].g +
				4 * buffer0[x + 1 + img->width * (y + 1)].g +
				2 * buffer0[x + 2 + img->width * (y + 1)].g +
				buffer0[x + img->width * (y + 2)].g +
				2 * buffer0[x + 1 + img->width * (y + 2)].g +
				buffer0[x + 2 + img->width * (y + 2)].g;	
			uint32_t bgacc =
				buffer0[x + img->width * y].b +
				2 * buffer0[x + 1 + img->width * y].b +
				buffer0[x + 2 + img->width * y].b +
				2 * buffer0[x + img->width * (y + 1)].b +
				4 * buffer0[x + 1 + img->width * (y + 1)].b +
				2 * buffer0[x + 2 + img->width * (y + 1)].b +
				buffer0[x + img->width * (y + 2)].b +
				2 * buffer0[x + 1 + img->width * (y + 2)].b +
				buffer0[x + 2 + img->width * (y + 2)].b;	
			

			/* Find edge */
			double 
				 rhnorm = rhacc / 255.0 / 3.0,
				 ghnorm = ghacc / 255.0 / 3.0,
				 bhnorm = bhacc / 255.0 / 3.0,
				 rvnorm = rvacc / 255.0 / 3.0,
				 gvnorm = gvacc / 255.0 / 3.0,
				 bvnorm = bvacc / 255.0 / 3.0,

				 redge = sqrt(rhnorm * rhnorm + rvnorm * rvnorm),
				 gedge = sqrt(ghnorm * ghnorm + gvnorm * gvnorm),
				 bedge = sqrt(bhnorm * bhnorm + bvnorm * bvnorm),

				 edge = redge;

			if (gedge > edge)
				edge = gedge;
			if (bedge > edge)
				edge = bedge;

			edge /= sqrt(2);


			/* Denoise */
			double 
				 rgauss = rgacc / 16.0,
				 ggauss = ggacc / 16.0,
				 bgauss = bgacc / 16.0;

			bmp_pixel* org_pix = &buffer0[x + 1 + img->width * (y + 1)];

			buffer1[x + new_w * y] = (bmp_pixel){
				.r = (uint8_t)(org_pix->r + edge * (rgauss - org_pix->r)),
				.g = (uint8_t)(org_pix->g + edge * (ggauss - org_pix->g)),
				.b = (uint8_t)(org_pix->b + edge * (bgauss - org_pix->b))
			};
		}

	img->buffer = buffer1;
	img->width  = new_w;
	img->height = new_h;

	free(buffer0);
}