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

void gfx_mat4_set_lookat(mat4 matrix, const float eyeX, const float eyeY, const float eyeZ, const float centerX, const float centerY, const float centerZ, const float upX, const float upY, const float upZ)
{
	float length;
	mat4 t;

	if(!matrix)
		return;

	gfx_mat4_set_identity(matrix);

	length = sqrt(upX * upX + upY * upY + upZ * upZ);
	matrix[1] = upX / length;
	matrix[5] = upY / length;
	matrix[9] = upZ / length;

	length = sqrt((centerX - eyeX) * (centerX - eyeX) + (centerY - eyeY) * (centerY - eyeY) + (centerZ - eyeZ) * (centerZ - eyeZ));
	matrix[2] = -(centerX - eyeX) / length;
	matrix[6] = -(centerY - eyeY) / length;
	matrix[10] = -(centerZ - eyeZ) / length;

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

	gfx_mat4_set_translate(t, -eyeX, -eyeY, -eyeZ);
	gfx_mat4_multiply(matrix, matrix, t);

}

void gfx_mat4_set_frustrum(mat4 matrix, const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal)
{
	float temp, temp2, temp3, temp4;
	temp = 2.0 * nearVal;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = farVal - nearVal;

	if(!matrix)
		return;

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
	matrix[10] = (-farVal - nearVal) / temp4;
	matrix[11] = -1.0;
	matrix[12] = 0.0;
	matrix[13] = 0.0;
	matrix[14] = (-temp * farVal) / temp4;
	matrix[15] = 0.0;
}

void gfx_mat4_set_perspective(mat4 matrix, const float fovy, const float aspect, const float zNear, const float zFar)
{
	float ymax, xmax;
	ymax = zNear * tanf(fovy * M_PI / 360.0);
	xmax = ymax * aspect;

	if(!matrix)
		return;

	gfx_mat4_set_frustrum(matrix, -xmax, xmax, -ymax, ymax, zNear, zFar);
}

void gfx_mat4_set_ortho(mat4 matrix, const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal)
{
	if(!matrix)
		return;

	gfx_mat4_set_identity(matrix);

	matrix[0] = 2.0 / (right - left);
	matrix[5] = 2.0 / (top - bottom);
	matrix[10] = -2.0 / (farVal - nearVal);

	matrix[12] = -(right + left) / (right - left);
	matrix[13] = -(top + bottom) / (top - bottom);
	matrix[14] = -(farVal + nearVal) / (farVal - nearVal);
}

