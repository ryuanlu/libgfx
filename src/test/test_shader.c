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
	gfx_text_layout* text = NULL;
	gfx_program *pg = NULL;
	gfx_vertex_attribute *attrib[3];
	gfx_vertex_array *array;
	mat4 matrix;

	float vertices[12] =
	{
		-1.0, -1.0, 0.0,
		1.0, -1.0, 0.0,
		-1.0, 1.0, 0.0,
		1.0, 1.0, 0.0,
	};
	float texcoord[8] =
	{
		0.0, 0.0,
		1.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
	};
	int index[4] = { 0, 1, 2, 3};

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
	gfx_framebuffer_clear(0.0, 0.0, 1.0, 1.0, GFX_CLEAR_COLOR_AND_DEPTH);


	pg = gfx_program_new_from_integrated(GFX_SIMPLE_TEXTURE_PROGRAM);
	gfx_program_use(pg);

	attrib[0] = gfx_vertex_attribute_new(GFX_ATTRIBUTE_ARRAY, vertices, GFX_FLOAT, 12, GFX_STATIC_DRAW);
	attrib[1] = gfx_vertex_attribute_new(GFX_ATTRIBUTE_ARRAY, texcoord, GFX_FLOAT, 8, GFX_STATIC_DRAW);
	attrib[2] = gfx_vertex_attribute_new(GFX_INDEX_ARRAY, index, GFX_UNSIGNED_INT, 4, GFX_STATIC_DRAW);

	array = gfx_vertex_array_new();
	gfx_vertex_array_bind(array);

	gfx_vertex_array_attach_attribute(attrib[0], 0, 3, GFX_FLOAT, FALSE, 0, NULL);
	gfx_vertex_array_attach_attribute(attrib[1], 1, 2, GFX_FLOAT, FALSE, 0, NULL);
	gfx_vertex_array_attach_index(attrib[2]);

	text = gfx_text_layout_new(640, 480);

	gfx_text_layout_set_pango_markup(text, "<span font='Sans 32'>あいうえお\nかきくけこ</span>");
	gfx_text_layout_set_background(text, 1.0, 1.0, 1.0, 0.0);
	gfx_text_layout_set_foreground(text, 1.0, 1.0, 0.0, 1.0);
	gfx_text_layout_set_alignment(text, PANGO_ALIGN_CENTER);
	gfx_text_layout_set_indent(text, 100);
	gfx_text_layout_set_line_spacing(text, 100);
	gfx_text_layout_render(text);

	gfx_texture_bind(0, (gfx_texture*)text);

	gfx_mat4_set_identity(matrix);
	gfx_program_set_uniform_mat4("model_matrix", matrix);
	gfx_program_set_uniform_mat4("view_matrix", matrix);
	gfx_program_set_uniform_mat4("projection_matrix", matrix);

	gfx_vertex_array_draw(GFX_TRIANGLE_STRIP, 0, 4);

	g_signal_connect(window, "destroy", gtk_main_quit, NULL);
	g_signal_connect(canvas, "expose-event", G_CALLBACK(glcanvas_handler), NULL);

	gtk_main();

	/* Release resource */

	gfx_text_layout_delete(&text);
	gfx_framebuffer_delete(&fb);
	gfx_context_delete(&ctx);

	return EXIT_SUCCESS;
}
