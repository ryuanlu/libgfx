#include <stdlib.h>
#include <GL/glew.h>
#include "gfx.h"
#include "framebuffer.h"
#include "image.h"
#include "texture.h"

gfx_framebuffer gfx_framebuffer_new(const int width, const int height, const gfx_pixel_format format, const int multisample, const int samples)
{
	gfx_framebuffer fb = NULL;

	if(width<1||height<1||!gfx_is_valid_pixel_format(format))
		return NULL;

	fb = calloc(1, sizeof(struct gfx_framebuffer));

	fb->width = width;
	fb->height = height;
	fb->format = format;

	glGenFramebuffers(1, &fb->fbo);

	glGenRenderbuffers(1, &fb->builtin_color_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, fb->builtin_color_buffer);

	if(multisample)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, gfx_get_gl_internal_format(format), width, height);
	else glRenderbufferStorage(GL_RENDERBUFFER, gfx_get_gl_internal_format(format), width, height);


	glGenRenderbuffers(1, &fb->builtin_depth_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER, fb->builtin_depth_buffer);

	if(multisample)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT, width, height);
	else glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);


	gfx_framebuffer_attach_texture(fb, GFX_ATTACH_COLOR_BUFFER, NULL);
	gfx_framebuffer_attach_texture(fb, GFX_ATTACH_DEPTH_BUFFER, NULL);

	return fb;
}

gfx_result gfx_framebuffer_attach_texture(gfx_framebuffer framebuffer, const gfx_fb_attachment target, const gfx_texture texture)
{
	GLenum attach;
	GLuint object;

	if(!framebuffer || target >= GFX_NUMBER_OF_ATTACH_DEPTH_BUFFER || target < 0)
		return GFX_ERROR;

	if(texture && (framebuffer->width != texture->width || framebuffer->height != texture->height))
		return GFX_ERROR;

	if(texture->target != GL_TEXTURE_2D)
		return GFX_ERROR;

	switch(target)
	{
	case GFX_ATTACH_COLOR_BUFFER:
		attach = GL_COLOR_ATTACHMENT0;
		framebuffer->bind_color_buffer = (texture == NULL ? framebuffer->builtin_color_buffer : texture->object);
		object = framebuffer->bind_color_buffer;
		break;
	case GFX_ATTACH_DEPTH_BUFFER:
		attach = GL_DEPTH_ATTACHMENT;
		framebuffer->bind_depth_buffer = (texture == NULL ? framebuffer->builtin_depth_buffer : texture->object);
		object = framebuffer->bind_depth_buffer;
		break;
	default:
		break;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer->fbo);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, attach, GL_TEXTURE_2D, object);

	return GFX_SUCCESS;
}

gfx_result gfx_framebuffer_bind(const gfx_framebuffer framebuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer ? framebuffer->fbo : 0);

	return GFX_SUCCESS;
}
