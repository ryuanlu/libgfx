#include <math.h>
#include <string.h>
#include "gfx.h"

#define GFX_PI (3.1415926)

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
	mat4 a, b;
	int row, col, i;

	if(!output || !A || !B)
		return;

	memcpy(a, A, sizeof(mat4));
	memcpy(b, B, sizeof(mat4));

	for(col = 0; col < 4; ++col)
	{
		for(row = 0; row < 4; ++row)
		{
			output[col * 4 + row] = 0.0f;

			for(i = 0; i < 4; ++i)
			{
				output[col * 4 + row] += a[row + 4 * i] * b[col * 4 + i];
			}
		}
	}

	for(i = 0; i<16; ++i)
		output[i] /= output[15];
}

void gfx_mat4_set_translate(mat4 matrix, const float x, const float y, const float z)
{
	if(!matrix)
		return;

	gfx_mat4_set_identity(matrix);
	matrix[12] = x;
	matrix[13] = y;
	matrix[14] = z;
}

void gfx_mat4_set_rotate(mat4 matrix, const float x, const float y, const float z, const float angle)
{
	float ax, ay, az;
	float mag;
	float b;
	float c;
	float ac;
	float s;

	if(!matrix)
		return;

	gfx_mat4_set_identity(matrix);

	mag = sqrt(x * x + y * y + z * z);
	ax = x / mag;
	ay = y / mag;
	az = z / mag;

	b = angle / 180.0 * GFX_PI;
	c = cos(b);
	ac = 1.00f - c;
	s = sin(b);

	matrix[0] = ax * ax * ac + c;
	matrix[1] = ax * ay * ac + az * s;
	matrix[2] = ax * az * ac - ay * s;

	matrix[4] = ay * ax * ac - az * s;
	matrix[5] = ay * ay * ac + c;
	matrix[6] = ay * az * ac + ax * s;

	matrix[8] = az * ax * ac + ay * s;
	matrix[9] = az * ay * ac - ax * s;
	matrix[10] = az * az * ac + c;

}

void gfx_mat4_set_scale(mat4 matrix, const float x, const float y, const float z)
{
	if(!matrix)
		return;

	gfx_mat4_set_identity(matrix);
	matrix[0] = x;
	matrix[5] = y;
	matrix[10] = z;

}

void gfx_mat4_set_lookat(mat4 matrix, const float from_x, const float from_y, const float from_z, const float to_x, const float to_y, const float to_z,	const float up_x, const float up_y, const float up_z)
{
	if(!matrix)
		return;
}

