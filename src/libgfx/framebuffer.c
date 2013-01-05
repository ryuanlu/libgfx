#include <stdlib.h>
#include <GL/glew.h>
#include "gfx.h"
#include "context.h"
#include "framebuffer.h"
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

	glDeleteRenderbuffers(1, &fb->builtin_color_buffer);
	glDeleteRenderbuffers(1, &fb->builtin_depth_buffer);
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
		framebuffer->bind_color_texture = texture;
		attach = GL_COLOR_ATTACHMENT0;
		object = (texture == NULL) ? framebuffer->builtin_color_buffer : framebuffer->bind_color_texture->object;
		break;
	case GFX_ATTACH_DEPTH_BUFFER:
		framebuffer->bind_depth_texture = texture;
		attach = GL_DEPTH_ATTACHMENT;
		object = (texture == NULL) ? framebuffer->builtin_depth_buffer : framebuffer->bind_depth_texture->object;
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

void gfx_framebuffer_clear(const float red, const float green, const float blue, const float alpha, const gfx_fb_clear_target target)
{
	GLbitfield mask = 0;
	glClearColor(red, green, blue, alpha);
	mask |= (target & GFX_CLEAR_COLOR_BUFFER) ? GL_COLOR_BUFFER_BIT : 0;
	mask |= (target & GFX_CLEAR_DEPTH_BUFFER) ? GL_DEPTH_BUFFER_BIT : 0;
	glClear(mask);

	if(gfx_current_context->current_framebuffer->bind_color_texture && (target & GFX_CLEAR_COLOR_BUFFER))
		gfx_current_context->current_framebuffer->bind_color_texture->modified = 1;
	if(gfx_current_context->current_framebuffer->bind_depth_texture && (target & GFX_CLEAR_DEPTH_BUFFER))
		gfx_current_context->current_framebuffer->bind_depth_texture->modified = 1;

}
