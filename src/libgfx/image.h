#ifndef IMAGE_H_
#define IMAGE_H_

#include <pango/pangocairo.h>
#include "gfx.h"

struct gfx_image
{
	unsigned int width;
	unsigned int height;
	unsigned int depth;
	unsigned int stride_size;
	unsigned int size;
	gfx_pixel_format format;
	unsigned char* data;

	cairo_surface_t* cairo_surface;
	cairo_t* cairo_context;
	PangoLayout* text_layout;
};

#define gfx_is_valid_pixel_format(format) (format >= 0 && format < GFX_NUMBER_OF_PIXELFORMAT)
int gfx_image_stride(const int width, const gfx_pixel_format format);

#endif /* IMAGE_H_ */
