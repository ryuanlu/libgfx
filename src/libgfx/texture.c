#include <stdlib.h>
#include <GL/glew.h>
#include "texture.h"
#include "framebuffer.h"
#include "context.h"

GLenum gfx_get_gl_format(const gfx_pixel_format format)
{
	GLenum table[] =
	{
		GL_RGBA,
		GL_RGB,
		GL_RG,
		GL_RED,
		GL_BGRA,
		GL_BGR,
		GL_DEPTH_COMPONENT,
		GL_DEPTH_COMPONENT,
		GL_DEPTH_COMPONENT,
		GL_DEPTH_COMPONENT,
	};

	return table[format];
}

GLenum gfx_get_gl_internal_format(const gfx_pixel_format format)
{
	GLenum table[] =
	{
		GL_RGBA,
		GL_RGB,
		GL_RG,
		GL_RED,
		GL_RGBA,
		GL_RGB,
		GL_DEPTH_COMPONENT,
		GL_DEPTH_COMPONENT,
		GL_DEPTH_COMPONENT,
		GL_DEPTH_COMPONENT,
	};

	return table[format];
}

GLenum gfx_get_gl_data_type(const gfx_pixel_format format)
{
	switch(format)
	{
	case GFX_PIXELFORMAT_DEPTH32F:
		return GL_FLOAT;
	case GFX_PIXELFORMAT_DEPTH32:
		return GL_UNSIGNED_INT;
	case GFX_PIXELFORMAT_DEPTH16:
		return GL_UNSIGNED_SHORT;
	default:
		return GL_UNSIGNED_BYTE;
	}
}

static int sizeof_pixel(gfx_pixel_format format)
{
	int pixelsize_table[] =
	{
		4, 3, 2, 1, 4, 3, 4, 4, 2, 1, 0
	};

	return gfx_is_valid_pixel_format(format) ? pixelsize_table[format]: 0;
}

gfx_texture gfx_texture_new(const int width, const int height, const int depth, const gfx_pixel_format format, const unsigned char* data)
{
	GLenum texture_target = GL_TEXTURE_1D;
	gfx_texture texture = NULL;
	int width_in_bytes;

	if(!gfx_is_valid_pixel_format(format) || width <= 0)
		return NULL;

	texture = calloc(1, sizeof(struct gfx_texture));

	if(height > 1)
		texture_target = GL_TEXTURE_2D;
	if(depth > 1)
		texture_target = GL_TEXTURE_3D;

	texture->target = texture_target;
	texture->width = width;
	texture->height = (height < 1) ? 1 : height;
	texture->depth = (depth < 1) ? 1 : depth;
	texture->format = format;

	width_in_bytes = width * sizeof_pixel(format);
	texture->stride_size = width_in_bytes + ((width_in_bytes % 4) ? 4 - (width_in_bytes % 4) : 0);
	texture->size = texture->stride_size * texture->height * texture->depth;

	glGenTextures(1, &texture->object);
	gfx_texture_bind(0, texture);

	switch(texture_target)
	{
	case GL_TEXTURE_1D:
		glTexImage1D(texture_target, 0, gfx_get_gl_internal_format(format), width, 0, gfx_get_gl_format(format), gfx_get_gl_data_type(format), data);
		break;
	case GL_TEXTURE_2D:
		glTexImage2D(texture_target, 0, gfx_get_gl_internal_format(format), width, height, 0, gfx_get_gl_format(format), gfx_get_gl_data_type(format), data);
		break;
	case GL_TEXTURE_3D:
		glTexImage3D(texture_target, 0, gfx_get_gl_internal_format(format), width, height, depth, 0, gfx_get_gl_format(format), gfx_get_gl_data_type(format), data);
		break;
	}

	glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	return texture;
}

gfx_result gfx_texture_delete(gfx_texture* texture)
{
	gfx_texture target = NULL;

	if(!texture||!*texture)
		return GFX_ERROR;

	target = *texture;

	glDeleteTextures(1, &target->object);

	if(target->pango_layout)
		g_object_unref(target->pango_layout);
	if(target->cairo_context)
		cairo_destroy(target->cairo_context);
	if(target->cairo_surface)
		cairo_surface_destroy(target->cairo_surface);
	if(target->data)
		free(target->data);

	free(target);
	*texture = NULL;

	return GFX_SUCCESS;
}

void gfx_texture_upload(gfx_texture texture, const gfx_pixel_format format, const unsigned char* data)
{
	if(!texture)
		return;

	gfx_texture_bind(0, texture);

	switch(texture->target)
	{
	case GL_TEXTURE_1D:
		glTexSubImage1D(texture->target, 0, 0, texture->width, gfx_get_gl_format(format), gfx_get_gl_data_type(format), data);
		break;
	case GL_TEXTURE_2D:
		glTexSubImage2D(texture->target, 0, 0, 0, texture->width, texture->height, gfx_get_gl_format(format), gfx_get_gl_data_type(format), data);
		break;
	case GL_TEXTURE_3D:
		glTexSubImage3D(texture->target, 0, 0, 0, 0, texture->width, texture->height, texture->depth, gfx_get_gl_format(format), gfx_get_gl_data_type(format), data);
		break;
	}

	return;
}

void gfx_texture_download(gfx_texture texture)
{
	if(!texture)
		return;

	if(!texture->data)
	{
		texture->data = calloc(1, texture->size);
		texture->modified = 1;
	}

	if(!texture->modified)
		return;

	glBindTexture(texture->target, texture->object);
	glGetTexImage(texture->target, 0, gfx_get_gl_format(texture->format), gfx_get_gl_data_type(texture->format), texture->data);

	texture->modified = 0;
}

gfx_result gfx_texture_generate_mipmaps(gfx_texture texture)
{
	if(texture->enabled_mipmaps)
		return GFX_ERROR;

	gfx_texture_bind(0, texture);
	glTexParameteri(texture->target, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(texture->target);

	texture->enabled_mipmaps = GL_TRUE;

	return GFX_SUCCESS;
}

gfx_result gfx_texture_bind(const int texture_unit, const gfx_texture texture)
{
	if(texture_unit < 0 || texture_unit >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
		return GFX_ERROR;

	glActiveTexture(GL_TEXTURE0 + texture_unit);
	glBindTexture(texture->target, texture->object);

	return GFX_SUCCESS;
}

gfx_result gfx_texture_copy_from_framebuffer(gfx_texture texture, const gfx_fb_attachment target)
{
	GLuint tmpfbo;
	GLenum attach;
	GLuint object;
	GLenum buffer_bit;
	gfx_framebuffer framebuffer = gfx_current_context->current_framebuffer;

	if(!texture || !framebuffer)
		return GFX_ERROR;

	switch(target)
	{
	case GFX_ATTACH_COLOR_BUFFER:
		object = framebuffer->bind_color_texture->object;
		attach = GL_COLOR_ATTACHMENT0;
		buffer_bit = GL_COLOR_BUFFER_BIT;
		break;
	case GFX_ATTACH_DEPTH_BUFFER:
		object = framebuffer->bind_depth_texture->object;
		attach = GL_DEPTH_ATTACHMENT;
		buffer_bit = GL_DEPTH_BUFFER_BIT;
		break;
	default:
		break;
	}

	glGenFramebuffers(1, &tmpfbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, tmpfbo);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, attach, texture->object, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer->fbo);
	glBlitFramebuffer(0, 0, framebuffer->width, framebuffer->height, 0, 0, texture->width, texture->height, buffer_bit, GL_LINEAR);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer->fbo);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &tmpfbo);

	return GFX_SUCCESS;
}

void gfx_texture_draw_pango_markup(gfx_texture texture, const int x, const int y, const int width, const int wrapping, const char* markup)
{
	if(!texture)
		return;
	if(texture->format != GFX_PIXELFORMAT_BGRA32)
		return;

	gfx_texture_download(texture);

	if(!texture->cairo_surface)
		texture->cairo_surface = cairo_image_surface_create_for_data(texture->data, CAIRO_FORMAT_ARGB32, texture->width, texture->height, cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, texture->width));
	if(!texture->cairo_context)
		texture->cairo_context = cairo_create(texture->cairo_surface);
	if(!texture->pango_layout)
		texture->pango_layout = pango_cairo_create_layout(texture->cairo_context);

	cairo_set_source_rgba(texture->cairo_context, 1.0, 1.0, 1.0, 1.0);
	cairo_scale(texture->cairo_context, 1.0, -1.0);
	cairo_translate(texture->cairo_context, x, -y);
	pango_layout_set_markup(texture->pango_layout, markup, -1);
	pango_layout_set_width(texture->pango_layout, wrapping ? width * PANGO_SCALE : -1);
	pango_cairo_show_layout(texture->cairo_context, texture->pango_layout);
	cairo_translate(texture->cairo_context, -x, y);
	cairo_scale(texture->cairo_context, 1.0, -1.0);

	gfx_texture_upload(texture, texture->format, texture->data);
}
