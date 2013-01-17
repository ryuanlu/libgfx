#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"

int main(int argc, char **argv)
{
	int i;
	mat4 m = {};

	gfx_mat4_set_identity(m);

	for(i = 0; i < 16; ++i)
		fprintf(stderr, "%f ", m[i]);



	return EXIT_SUCCESS;
}
