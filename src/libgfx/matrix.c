#include "gfx.h"


void gfx_mat4_set_identity(mat4 matrix)
{
	int i;

	if(!matrix)
		return;

	for(i = 0; i < 16; ++i)
		matrix[i] = (i % 5) ? 0.0f : 1.0f;
}

void gfx_mat4_multiply(mat4 output, const mat4 A, const mat4 B)
{
	if(!output)
		return;

}

void gfx_mat4_set_translate(mat4 matrix, const float x, const float y, const float z)
{
	if(!matrix)
		return;

	gfx_mat4_set_identity(matrix);
}

void gfx_mat4_set_rotate(mat4 matrix, const float x, const float y, const float z, const float angle)
{
	if(!matrix)
		return;

}

void gfx_mat4_set_scale(mat4 matrix, const float x, const float y, const float z)
{
	if(!matrix)
		return;

}

void gfx_mat4_set_lookat(mat4 matrix, const float from_x, const float from_y, const float from_z, const float to_x, const float to_y, const float to_z,	const float up_x, const float up_y, const float up_z)
{
	if(!matrix)
		return;
}

