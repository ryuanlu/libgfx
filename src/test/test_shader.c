#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"

int main(int argc, char **argv)
{
	gfx_context *ctx = NULL;
	gfx_program *pg = NULL;

	gfx_init(&argc, &argv);
	ctx = gfx_context_new(NULL);

	pg = gfx_program_new_from_integrated(GFX_SIMPLE_COLOR_PROGRAM);
	gfx_program_delete(&pg);

	gfx_context_make_current(ctx);
	gfx_context_delete(&ctx);


	return EXIT_SUCCESS;
}
