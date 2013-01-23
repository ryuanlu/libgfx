#include <stdlib.h>
#include <GL/glew.h>
#include "vertex.h"
#include "context.h"
#include "gfx.h"

const static GLenum type_table[] = { GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT, GL_FLOAT, GL_DOUBLE };


gfx_vertex_attribute *gfx_vertex_attribute_new(const gfx_array_type array_type, void *data, const gfx_data_type data_type, const int count, const gfx_draw_hint hint)
{
	gfx_vertex_attribute *vertex_attribute = NULL;
	const int size_table[] = { 1, 1, 2, 2, 4, 4, 4, 8 };
	const GLenum hint_table[] = { GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW };

	if(!data)
		return NULL;

	vertex_attribute = calloc(1, sizeof(struct gfx_vertex_attribute));
	glGenBuffers(1, &(vertex_attribute->vbo));
	glBindBuffer((!array_type) ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, vertex_attribute->vbo);
	glBufferData((!array_type) ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER, count * size_table[data_type], data, hint_table[hint]);

	vertex_attribute->buffer = data;
	vertex_attribute->array_type = array_type;
	vertex_attribute->data_type = data_type;
	vertex_attribute->count = count;

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
	gfx_vertex_array *vertex_array = NULL;

	if(!gfx_current_context || !gfx_current_context->current_vertex_array)
		return;

	if(attribute->array_type != GFX_ATTRIBUTE_ARRAY)
		return;

	vertex_array = gfx_current_context->current_vertex_array;
	glBindVertexArray(vertex_array->vao);

	glBindBuffer(GL_ARRAY_BUFFER, attribute->vbo);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type_table[attribute->data_type], normalized, stride, pointer);

	if(vertex_array->vertex_count == 0 || vertex_array->vertex_count > attribute->count)
	{
		vertex_array->vertex_count = attribute->count;
	}

}

void gfx_vertex_array_attach_index(const gfx_vertex_attribute *attribute)
{
	if(!gfx_current_context || !gfx_current_context->current_vertex_array)
		return;

	glBindVertexArray(gfx_current_context->current_vertex_array->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, attribute->vbo);
	gfx_current_context->current_vertex_array->has_index_array = 1;
	gfx_current_context->current_vertex_array->index_type = attribute->data_type;

}


void gfx_vertex_array_draw(const gfx_draw_mode mode, const int start, const int count)
{
	const GLenum mode_table[] = { GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES, GL_TRIANGLE_STRIP };
	gfx_vertex_array *vertex_array = NULL;

	if(!gfx_current_context || !gfx_current_context->current_vertex_array)
		return;

	vertex_array = gfx_current_context->current_vertex_array;

	if(vertex_array->has_index_array)
	{
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(vertex_array->vertex_count);
		glDrawElements(mode_table[mode], count, type_table[vertex_array->index_type], NULL);
	}else
	{
		glDrawArrays(mode_table[mode], start, count);
	}
}
