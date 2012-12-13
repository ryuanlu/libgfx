#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"


int main(int argc, char **argv)
{
	gfx_context ctx = NULL;

	gfx_init(&argc, &argv);
	ctx = gfx_context_new(NULL);
	gfx_context_make_current(ctx);
	gfx_context_delete(&ctx);


	return EXIT_SUCCESS;
}
