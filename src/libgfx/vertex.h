#ifndef VERTEX_H_
#define VERTEX_H_

#include "gfx.h"

struct gfx_vertex_attribute
{
	unsigned int vbo;
	void *buffer;
	gfx_array_type type;
};

struct gfx_vertex_array
{
	unsigned int vao;
	int has_index_array;
};

#endif /* VERTEX_H_ */
