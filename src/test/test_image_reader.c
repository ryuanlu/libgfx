#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include "gfx.h"

gfx_framebuffer *fb = NULL;
GtkWidget *canvas = NULL;

int glcanvas_handler(GtkWidget *widget, GdkEvent *event)
{
	gfx_glx_blit_framebuffer(fb, GDK_DRAWABLE_XID(canvas->window));
	return 0;
}

int main(int argc, char **argv)
{
	gfx_context *ctx = NULL;
	GtkWidget *window = NULL;
	gfx_texture *tex = NULL;
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

	fb = gfx_framebuffer_new(480, 480, GFX_PIXELFORMAT_BGRA32, 0, 0);
	gfx_framebuffer_bind(fb);
	tex = gfx_texture_new_from_file(GFX_IMAGE_FORMAT_PNG, "test.png");

	gfx_framebuffer_attach_texture(GFX_ATTACH_COLOR_BUFFER, tex);

	g_signal_connect(window, "destroy", gtk_main_quit, NULL);
	g_signal_connect(canvas, "expose-event", G_CALLBACK(glcanvas_handler), NULL);

	gtk_main();

	/* Release resource */

	gfx_texture_delete(&tex);
	gfx_framebuffer_delete(&fb);
	gfx_context_delete(&ctx);

	return EXIT_SUCCESS;
}
