#include <stdlib.h>
#include <GL/glew.h>
#include "vertex.h"
#include "context.h"
#include "gfx.h"


gfx_vertex_attribute *gfx_vertex_attribute_new(const gfx_array_type type, void *data, const int size, const int hint)
{
	gfx_vertex_attribute *vertex_attribute = NULL;

	if(!data)
		return NULL;

	vertex_attribute = calloc(1, sizeof(struct gfx_vertex_attribute));
	glGenBuffers(1, &(vertex_attribute->vbo));
	glBindBuffer((!type) ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, vertex_attribute->vbo);
	glBufferData((!type) ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, size, data, hint);

	vertex_attribute->buffer = data;
	vertex_attribute->type = type;

	return vertex_attribute;
}


gfx_vertex_array *gfx_vertex_array_new(void)
{
	gfx_vertex_array *vertex_array = NULL;

	vertex_array = calloc(1, sizeof(struct gfx_vertex_array));
	glGenVertexArrays(1, &(vertex_array->vao));

	return vertex_array;
}

void gfx_vertex_array_bind(gfx_vertex_array *vertex_array)
{
	if(!vertex_array)
		return;

	glBindVertexArray(vertex_array->vao);
	gfx_current_context->current_vertex_array = vertex_array;
}


void gfx_vertex_array_attach_attribute(const gfx_vertex_attribute *attribute, const int index, const int size, const int type, const int normalized, const int stride, const void *pointer)
{
	if(!gfx_current_context || !gfx_current_context->current_vertex_array)
		return;

	glBindVertexArray(gfx_current_context->current_vertex_array->vao);

	glBindBuffer(GL_ARRAY_BUFFER, attribute->vbo);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);

	if(attribute->type == GFX_INDEX_ARRAY)
		gfx_current_context->current_vertex_array->has_index_array = 1;
}
