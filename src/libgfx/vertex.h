#ifndef VERTEX_H_
#define VERTEX_H_

#include "gfx.h"

struct gfx_vertex_attribute
{
	unsigned int vbo;
	void *buffer;
	gfx_array_type array_type;
	gfx_data_type data_type;
	int count;
};

struct gfx_vertex_array
{
	unsigned int vao;
	int has_index_array;
	gfx_data_type index_type;
	int vertex_count;

};

#endif /* VERTEX_H_ */
