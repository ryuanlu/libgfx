#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"


int main(int argc, char **argv)
{
	gfx_context ctx = NULL;
	gfx_image img = NULL;
	gfx_texture tex = NULL;

	gfx_init(&argc, &argv);
	ctx = gfx_context_new(NULL);
	gfx_context_make_current(ctx);

	img = gfx_image_new(31, 31, 0, GFX_PIXELFORMAT_BGRA32, NULL);
	tex = gfx_texture_new_from_image(img);
	gfx_image_delete(&img);
	gfx_texture_delete(&tex);

	gfx_context_delete(&ctx);

	return EXIT_SUCCESS;
}
