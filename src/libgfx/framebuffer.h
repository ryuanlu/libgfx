#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "gfx.h"


typedef enum
{
	GFX_FRAMEBUFFER_BIND_RENDERBUFFER = 0,
	GFX_FRAMEBUFFER_BIND_TEXTURE
}gfx_framebuffer_bind_object;


struct gfx_framebuffer
{
	GLuint fbo;

	GLuint builtin_color_buffer;
	GLuint bind_color_buffer;
	gfx_framebuffer_bind_object bind_color_buffer_type;

	GLuint builtin_depth_buffer;
	GLuint bind_depth_buffer;
	gfx_framebuffer_bind_object bind_depth_buffer_type;

	int width;
	int height;
	gfx_pixel_format format;
};



#endif /* FRAMEBUFFER_H_ */
