#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "texture.h"


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

gfx_result gfx_image_copy_from_texture(gfx_image image, const gfx_texture texture)
{
	if(!texture || !image)
		return GFX_ERROR;

	if(image->width != texture->width)
		return GFX_ERROR;
	if(image->height < texture->height)
		return GFX_ERROR;
	if(image->depth < texture->depth)
		return GFX_ERROR;

	glBindTexture(texture->target, texture->object);
	glGetTexImage(texture->target, 0, gfx_get_gl_format(image->format), gfx_get_gl_data_type(image->format), image->data);

	return GFX_SUCCESS;
}

unsigned char* gfx_image_get_data(const gfx_image image)
{
	return image ? image->data : NULL;
}

int gfx_image_get_size(const gfx_image image)
{
	return image ? image->size : 0;
}

void gfx_image_draw_pango_markup(gfx_image image, const int x, const int y, const char* markup)
{
	if(!image)
		return;
	if(image->format != GFX_PIXELFORMAT_BGRA32)
		return;
	if(!image->cairo_surface)
		image->cairo_surface = cairo_image_surface_create_for_data(image->data, CAIRO_FORMAT_ARGB32, image->width, image->height, cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, image->width));
	if(!image->cairo_context)
		image->cairo_context = cairo_create(image->cairo_surface);
	if(!image->text_layout)
		image->text_layout = pango_cairo_create_layout(image->cairo_context);

	cairo_set_source_rgba(image->cairo_context, 1.0, 1.0, 1.0, 1.0);
	cairo_translate(image->cairo_context, x, y);
	pango_layout_set_markup(image->text_layout, markup, -1);
	pango_cairo_show_layout(image->cairo_context, image->text_layout);

}
