#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"

#define VERTEX_SHADER_FILENAME "vs.txt"
#define FRAGMENT_SHADER_FILENAME "fs.txt"

int main(int argc, char **argv)
{
	gfx_context *ctx = NULL;
	gfx_program *pg = NULL;

	gfx_init(&argc, &argv);
	ctx = gfx_context_new(NULL);

	pg = gfx_program_new
	(
		gfx_shader_new_from_file(GFX_VERTEX_SHADER, VERTEX_SHADER_FILENAME),
		NULL,
		gfx_shader_new_from_file(GFX_FRAGMENT_SHADER, FRAGMENT_SHADER_FILENAME)
	);

	gfx_program_delete(&pg);
	gfx_context_make_current(ctx);
	gfx_context_delete(&ctx);


	return EXIT_SUCCESS;
}
