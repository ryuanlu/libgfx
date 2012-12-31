#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include "gfx.h"

gfx_framebuffer fb = NULL;
GtkWidget* canvas = NULL;

int glcanvas_handler(GtkWidget *widget, GdkEvent *event)
{
	gfx_glx_blit_framebuffer(fb, GDK_DRAWABLE_XID(canvas->window));
}

int main(int argc, char **argv)
{
	gfx_context ctx = NULL;
	gfx_image img = NULL;
	gfx_texture tex = NULL;
	GtkWidget* window = NULL;
	FILE* fp;
	int r;

	/* Initialization */

	gfx_init(&argc, &argv);
	gtk_init(&argc, &argv);
	ctx = gfx_context_new(NULL);
	gfx_context_make_current(ctx);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	canvas = gtk_drawing_area_new();
	gtk_window_set_default_size(GTK_WINDOW(window), 640,480);
	gtk_widget_set_double_buffered(canvas, FALSE);

	gtk_widget_set_events(canvas, GDK_EXPOSURE_MASK);
	gtk_widget_realize(GTK_WIDGET(window));
	gtk_container_add(GTK_CONTAINER(window), canvas);
	gtk_widget_show_all(GTK_WIDGET(window));

	/* Create a new framebuffer to draw */

	fb = gfx_framebuffer_new(640, 480, GFX_PIXELFORMAT_BGRA32, 0, 0);
	gfx_framebuffer_bind(fb);
	tex = gfx_texture_new(640, 480, 0, GFX_PIXELFORMAT_BGRA32, NULL);
	gfx_framebuffer_attach_texture(GFX_ATTACH_COLOR_BUFFER, tex);
	gfx_framebuffer_clear(0.0f, 0.0f, 1.0f, 1.0f, GFX_CLEAR_COLOR_BUFFER);

	img = gfx_image_new(640, 480, 0, GFX_PIXELFORMAT_BGRA32, NULL);
	gfx_image_copy_from_texture(img, tex);
	gfx_image_draw_pango_markup(img, 0, 200, 100, 1, "<span font='Sans 24'>あいうえお</span>");
	gfx_image_draw_pango_markup(img, 0, 100, 100, 0, "<span font='Sans 24'>あいうえお</span>");
	gfx_texture_copy_from_image(tex, img, 0, 0, 0);

	g_signal_connect(window, "destroy", gtk_main_quit, NULL);
	g_signal_connect(canvas, "expose-event", G_CALLBACK(glcanvas_handler), NULL);

	gtk_main();


	/* Release resource */

	gfx_image_delete(&img);
	gfx_texture_delete(&tex);
	gfx_framebuffer_delete(&fb);
	gfx_context_delete(&ctx);

	return EXIT_SUCCESS;
}
