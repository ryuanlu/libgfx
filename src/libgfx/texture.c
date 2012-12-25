#include <stdlib.h>
#include <GL/glew.h>
#include "texture.h"
#include "image.h"

GLenum gfx_get_gl_format(const gfx_pixel_format format)
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

gfx_texture gfx_texture_new(const int width, const int height, const int depth, const gfx_pixel_format format, const unsigned char* data)
{
	GLenum texture_target = GL_TEXTURE_1D;
	gfx_texture texture = NULL;

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

	glGenTextures(1, &texture->object);
	glBindTexture(texture_target, texture->object);

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

	free(target);
	*texture = NULL;

	return GFX_SUCCESS;
}

gfx_texture gfx_texture_new_from_image(const gfx_image image)
{
	gfx_texture texture = NULL;
	if(!image)
		return NULL;

	texture = gfx_texture_new(image->width, image->height, image->depth, image->format, image->data);

	return texture;
}

gfx_result gfx_texture_copy_from_image(gfx_texture texture, const gfx_image image, const int offset_x, const int offset_y, const int offset_z)
{
	if(!texture || !image)
		return GFX_ERROR;

	glBindTexture(texture->target, texture->object);

	switch(texture->target)
	{
	case GL_TEXTURE_1D:
		glTexSubImage1D(texture->target, 0, offset_x, image->width, gfx_get_gl_format(image->format), gfx_get_gl_data_type(image->format), image->data);
		break;
	case GL_TEXTURE_2D:
		glTexSubImage2D(texture->target, 0, offset_x, offset_y, image->width, image->height, gfx_get_gl_format(image->format), gfx_get_gl_data_type(image->format), image->data);
		break;
	case GL_TEXTURE_3D:
		glTexSubImage3D(texture->target, 0, offset_x, offset_y, offset_z, image->width, image->height, image->depth, gfx_get_gl_format(image->format), gfx_get_gl_data_type(image->format), image->data);
		break;
	}

	return GFX_SUCCESS;
}
