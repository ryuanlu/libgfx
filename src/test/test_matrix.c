#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"

int main(int argc, char **argv)
{
	int i;
	mat4 m, n;

	gfx_mat4_set_translate(m, 3, 4, 5);
	gfx_mat4_set_rotate(n, 3, 4, 5, 67);
	gfx_mat4_multiply(m, m, n);

	for(i = 0; i < 16; ++i)
		fprintf(stderr, "%f ", m[i]);

	return EXIT_SUCCESS;
}
