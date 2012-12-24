#include <stdlib.h>
#include <string.h>
#include "image.h"

static int sizeof_pixel(gfx_pixel_format format)
{
	int pixelsize_table[] =
	{
		4, 3, 2, 1, 4, 3, 4, 4, 2, 1, 0
	};

	return gfx_is_valid_pixel_format(format) ? pixelsize_table[format]: 0;
}

int gfx_image_stride(const int width, const gfx_pixel_format format)
{
	int width_in_bytes = width * sizeof_pixel(format);
	return width_in_bytes + ((width_in_bytes % 4) ? 4 - (width_in_bytes % 4) : 0);
}

gfx_image gfx_image_new(const int width, const int height, const int depth, const gfx_pixel_format format, unsigned char* data)
{
	gfx_image image = NULL;

	if(!gfx_is_valid_pixel_format(format) || width <= 0)
		return image;

	image = calloc(1, sizeof(struct gfx_image));
	image->width = width;
	image->height = (height < 1) ? 1 : height;
	image->depth = (depth < 1) ? 1 : depth;
	image->format = format;
	image->stride_size = gfx_image_stride(width, format);
	image->size = image->stride_size * image->height * image->depth;
	image->data = data ? data : calloc(1, image->size);

	return image;
}

gfx_result gfx_image_delete(gfx_image* image)
{
	gfx_image target = NULL;

	if(!image || !*image)
		return GFX_ERROR;

	target = *image;

	if(target->data)
		free(target->data);

	free(target);
	*image = NULL;

	return GFX_SUCCESS;
}

gfx_result gfx_image_copy_from_memory(gfx_image image, const char* data)
{
	if(!image||!data)
		return GFX_ERROR;

	memcpy(image->data, data, image->size);

	return GFX_SUCCESS;
}

