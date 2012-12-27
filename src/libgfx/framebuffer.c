#include <stdlib.h>
#include <GL/glew.h>
#include "gfx.h"
#include "context.h"
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


	gfx_framebuffer_bind(fb);
	gfx_framebuffer_attach_texture(GFX_ATTACH_COLOR_BUFFER, NULL);
	gfx_framebuffer_attach_texture(GFX_ATTACH_DEPTH_BUFFER, NULL);
	gfx_framebuffer_bind(gfx_current_context->current_framebuffer);

	return fb;
}

gfx_result gfx_framebuffer_delete(gfx_framebuffer* framebuffer)
{
	gfx_framebuffer fb = *framebuffer;

	glDeleteRenderbuffers(1, &fb->bind_color_buffer);
	glDeleteRenderbuffers(1, &fb->bind_depth_buffer);
	glDeleteFramebuffers(1, &fb->fbo);

	free(fb);
	*framebuffer = NULL;

	return GFX_SUCCESS;
}

gfx_result gfx_framebuffer_attach_texture(const gfx_fb_attachment target, const gfx_texture texture)
{
	GLenum attach;
	GLuint object;

	gfx_framebuffer framebuffer = gfx_current_context->current_framebuffer;

	if(!framebuffer || target >= GFX_NUMBER_OF_ATTACH_DEPTH_BUFFER || target < 0)
		return GFX_ERROR;

	if(texture && (framebuffer->width != texture->width || framebuffer->height != texture->height || texture->target != GL_TEXTURE_2D))
		return GFX_ERROR;

	switch(target)
	{
	case GFX_ATTACH_COLOR_BUFFER:
		framebuffer->bind_color_buffer = (texture == NULL ? framebuffer->builtin_color_buffer : texture->object);
		framebuffer->bind_color_buffer_type = (texture == NULL ? GFX_FRAMEBUFFER_BIND_RENDERBUFFER : GFX_FRAMEBUFFER_BIND_TEXTURE);
		attach = GL_COLOR_ATTACHMENT0;
		object = framebuffer->bind_color_buffer;
		break;
	case GFX_ATTACH_DEPTH_BUFFER:
		framebuffer->bind_depth_buffer = (texture == NULL ? framebuffer->builtin_depth_buffer : texture->object);
		framebuffer->bind_depth_buffer_type = (texture == NULL ? GFX_FRAMEBUFFER_BIND_RENDERBUFFER : GFX_FRAMEBUFFER_BIND_TEXTURE);
		attach = GL_DEPTH_ATTACHMENT;
		object = framebuffer->bind_depth_buffer;
		break;
	default:
		break;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer->fbo);

	if(!texture)
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, attach, GL_RENDERBUFFER, object);
	else	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, attach, object, 0);

	return GFX_SUCCESS;
}

gfx_result gfx_framebuffer_bind(const gfx_framebuffer framebuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer ? framebuffer->fbo : 0);
	gfx_current_context->current_framebuffer = framebuffer;

	return GFX_SUCCESS;
}
