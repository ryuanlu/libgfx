#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GL/gl.h>
#include "gfx.h"

struct gfx_texture
{
	GLuint object;
	GLenum target;
	int width;
	int height;
	int depth;
};

#pragma GCC visibility push(hidden)
GLenum gfx_get_gl_format(const gfx_pixel_format format);
GLenum gfx_get_gl_internal_format(const gfx_pixel_format format);
GLenum gfx_get_gl_data_type(const gfx_pixel_format format);
#pragma GCC visibility pop

#endif /* TEXTURE_H_ */
