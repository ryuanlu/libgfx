#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "gfx.h"
#include "texture.h"

struct gfx_framebuffer
{
	GLuint fbo;

	GLuint builtin_color_buffer;
	gfx_texture bind_color_texture;

	GLuint builtin_depth_buffer;
	gfx_texture bind_depth_texture;

	int width;
	int height;
	gfx_pixel_format format;
};



#endif /* FRAMEBUFFER_H_ */
