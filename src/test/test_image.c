#include <stdio.h>
#include <stdlib.h>
#include "gfx.h"

int main(int argc, char **argv)
{
	gfx_context ctx = NULL;
	gfx_image img = NULL;
	gfx_texture tex = NULL;
	gfx_framebuffer fb = NULL;
	gfx_image out = NULL;
	FILE* fp;
	int r;

	/* Initialization */

	gfx_init(&argc, &argv);
	ctx = gfx_context_new(NULL);
	gfx_context_make_current(ctx);


	/* Create a new framebuffer to draw */

	fb = gfx_framebuffer_new(640, 480, GFX_PIXELFORMAT_RGBA32, 0, 0);
	gfx_framebuffer_bind(fb);
	gfx_framebuffer_clear(0.0f, 0.0f, 1.0f, 1.0f, GFX_CLEAR_COLOR_BUFFER);

	/* Create a new texture and copy pixels from the framebuffer */

	tex = gfx_texture_new(640, 480, 0, GFX_PIXELFORMAT_RGBA32, NULL);
	gfx_texture_copy_from_framebuffer(tex, GFX_ATTACH_COLOR_BUFFER);


	/* Create a new image object and download pixels from the texture */

	out = gfx_image_new(640, 480, 0, GFX_PIXELFORMAT_BGRA32, NULL);
	gfx_image_copy_from_texture(out, tex);
	gfx_image_draw_pango_markup(out, 10, 10, "<span font='Sans 24'>Pangoのことが</span><span font='Sans 24' color='red'><b>大好</b></span><span font='Sans 24'>きです</span>");

	img = gfx_image_new(640, 480, 0, GFX_PIXELFORMAT_RGBA32, NULL);
	gfx_texture_copy_from_image(tex, out, 0, 0, 0);
	gfx_image_copy_from_texture(img, tex);

	/* Write the image to a file */

	fp =fopen("test.raw", "w");
	fwrite(gfx_image_get_data(img), gfx_image_get_size(img), 1, fp);
	fclose(fp);


	/* Release resource */

	gfx_image_delete(&img);
	gfx_texture_delete(&tex);
	gfx_framebuffer_delete(&fb);
	gfx_context_delete(&ctx);

	return EXIT_SUCCESS;
}
