#include <math.h>
#include <string.h>
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

	b = angle / 180.0 * M_PI;
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

void gfx_mat4_set_lookat(mat4 matrix, const float eye_x, const float eye_y, const float eye_z, const float center_x, const float center_y, const float center_z, const float up_x, const float up_y, const float up_z)
{
	float length;
	mat4 t;

	if(!matrix)
		return;

	gfx_mat4_set_identity(matrix);

	length = sqrt(up_x * up_x + up_y * up_y + up_z * up_z);
	matrix[1] = up_x / length;
	matrix[5] = up_y / length;
	matrix[9] = up_z / length;

	length = sqrt((center_x - eye_x) * (center_x - eye_x) + (center_y - eye_y) * (center_y - eye_y) + (center_z - eye_z) * (center_z - eye_z));
	matrix[2] = -(center_x - eye_x) / length;
	matrix[6] = -(center_y - eye_y) / length;
	matrix[10] = -(center_z - eye_z) / length;

	matrix[0] = matrix[5] * matrix[10] - matrix[9] * matrix[6];
	matrix[4] = matrix[9] * matrix[2] - matrix[1] * matrix[10];
	matrix[8] = matrix[1] * matrix[6] - matrix[5] * matrix[2];
	length = sqrt(matrix[0] * matrix[0] + matrix[4] * matrix[4] + matrix[8] * matrix[8]);
	matrix[0] /= length;
	matrix[4] /= length;
	matrix[8] /= length;

	matrix[1] = matrix[6] * matrix[8] - matrix[10] * matrix[4];
	matrix[5] = matrix[10] * matrix[0] - matrix[2] * matrix[8];
	matrix[9] = matrix[2] * matrix[4] - matrix[6] * matrix[0];
	length = sqrt(matrix[1] * matrix[1] + matrix[5] * matrix[5] + matrix[9] * matrix[9]);
	matrix[1] /= length;
	matrix[5] /= length;
	matrix[9] /= length;

	gfx_mat4_set_translate(t, -eye_x, -eye_y, -eye_z);
	gfx_mat4_multiply(matrix, matrix, t);

}

void gfx_mat4_set_frustrum(float *matrix, float left, float right, float bottom, float top, float znear, float zfar)
{
	float temp, temp2, temp3, temp4;
	temp = 2.0 * znear;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;

	matrix[0] = temp / temp2;
	matrix[1] = 0.0;
	matrix[2] = 0.0;
	matrix[3] = 0.0;
	matrix[4] = 0.0;
	matrix[5] = temp / temp3;
	matrix[6] = 0.0;
	matrix[7] = 0.0;
	matrix[8] = (right + left) / temp2;
	matrix[9] = (top + bottom) / temp3;
	matrix[10] = (-zfar - znear) / temp4;
	matrix[11] = -1.0;
	matrix[12] = 0.0;
	matrix[13] = 0.0;
	matrix[14] = (-temp * zfar) / temp4;
	matrix[15] = 0.0;
}

