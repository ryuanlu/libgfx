#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "gfx.h"

struct gfx_framebuffer
{
	GLuint fbo;
	GLuint builtin_color_buffer;
	GLuint bind_color_buffer;
	GLuint builtin_depth_buffer;
	GLuint bind_depth_buffer;
	int width;
	int height;
	gfx_pixel_format format;
};



#endif /* FRAMEBUFFER_H_ */
