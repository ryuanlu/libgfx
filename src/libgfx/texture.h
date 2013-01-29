#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <cairo/cairo.h>
#include <GL/gl.h>
#include "gfx.h"

struct gfx_texture
{
	GLuint object;
	GLenum target;
	int width;
	int height;
	int depth;
	int enabled_mipmaps;

	int stride_size;
	int size;
	gfx_pixel_format format;
	unsigned char *data;
	int modified;

	cairo_surface_t *cairo_surface;
	cairo_t *cairo_context;
};

#define gfx_is_valid_pixel_format(format) (format >= 0 && format < GFX_NUMBER_OF_PIXELFORMAT)
#define gfx_is_valid_image_format(format) (format >= 0 && format < GFX_NUMBER_OF_IMAGE_FORMAT)
#pragma GCC visibility push(hidden)
GLenum gfx_get_gl_format(const gfx_pixel_format format);
GLenum gfx_get_gl_internal_format(const gfx_pixel_format format);
GLenum gfx_get_gl_data_type(const gfx_pixel_format format);

void gfx_texture_init(gfx_texture *texture, const int width, const int height, const int depth, const gfx_pixel_format format, const unsigned char *data);
void gfx_texture_deinit(gfx_texture *texture);

void gfx_texture_upload(const gfx_texture *texture, const gfx_pixel_format format, const unsigned char *data);
void gfx_texture_download(gfx_texture *texture);

#pragma GCC visibility pop

#endif /* TEXTURE_H_ */
