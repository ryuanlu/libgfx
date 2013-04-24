#include <stdlib.h>
#include <pango/pangocairo.h>
#include "texture.h"

struct gfx_text_layout
{
	struct gfx_texture _gfx_texture;
	PangoLayout *pango_layout;
	struct
	{
		float red;
		float green;
		float blue;
		float alpha;
	}foreground_color, background_color;
};

void gfx_text_layout_init(gfx_text_layout *textlayout, const int width, const int height)
{
	gfx_texture *texture = &(textlayout->_gfx_texture);
	gfx_texture_init(texture, width, height, 1, GFX_PIXELFORMAT_BGRA32, NULL);
	gfx_texture_download(texture);
	textlayout->pango_layout = pango_cairo_create_layout(texture->cairo_context);

	textlayout->foreground_color.red = 1.0f;
	textlayout->foreground_color.green = 1.0f;
	textlayout->foreground_color.blue = 1.0f;
	textlayout->foreground_color.alpha = 1.0f;

	textlayout->background_color.red = 0.0f;
	textlayout->background_color.green = 0.0f;
	textlayout->background_color.blue = 0.0f;
	textlayout->background_color.alpha = 0.0f;

	pango_layout_set_width(textlayout->pango_layout, width * PANGO_SCALE);
	pango_layout_set_height(textlayout->pango_layout, height * PANGO_SCALE);
}

void gfx_text_layout_deinit(gfx_text_layout *textlayout)
{
	if(!textlayout)
		return;

	g_object_unref(textlayout->pango_layout);
	gfx_texture_deinit(&(textlayout->_gfx_texture));
}

gfx_text_layout *gfx_text_layout_new(const int width, const int height)
{
	gfx_text_layout *textlayout = NULL;

	if(width < 2 || height < 2)
		return NULL;

	textlayout = calloc(1, sizeof(struct gfx_text_layout));
	gfx_text_layout_init(textlayout, width, height);

	return textlayout;
}

void gfx_text_layout_delete(gfx_text_layout **textlayout)
{
	gfx_text_layout *target;
	if(!textlayout||!*textlayout)
		return;

	target = *textlayout;
	gfx_text_layout_deinit(target);
	free(target);
	*textlayout = NULL;
}

void gfx_text_layout_render(gfx_text_layout *textlayout)
{
	int i;
	gfx_texture *texture = NULL;

	if(!textlayout)
		return;

	texture = &(textlayout->_gfx_texture);

	cairo_identity_matrix(texture->cairo_context);
	cairo_translate(texture->cairo_context, 0, texture->height);
	cairo_scale(texture->cairo_context, 1.0, -1.0);

	cairo_set_source_rgba
	(
		texture->cairo_context,
		textlayout->background_color.red,
		textlayout->background_color.green,
		textlayout->background_color.blue,
		textlayout->background_color.alpha
	);

	cairo_paint(texture->cairo_context);

	cairo_set_source_rgba
	(
		texture->cairo_context,
		textlayout->foreground_color.red,
		textlayout->foreground_color.green,
		textlayout->foreground_color.blue,
		textlayout->foreground_color.alpha
	);

	pango_cairo_show_layout(texture->cairo_context, textlayout->pango_layout);

	for (i = 0; i < textlayout->_gfx_texture.size / 4; ++i)
	{
		unsigned char *r, *g, *b, *a;

		b = &(textlayout->_gfx_texture.data[i * 4 + 0]);
		g = &(textlayout->_gfx_texture.data[i * 4 + 1]);
		r = &(textlayout->_gfx_texture.data[i * 4 + 2]);
		a = &(textlayout->_gfx_texture.data[i * 4 + 3]);

		if(*a && *a != 0xff)
		{
			*b = (*b * 255 + *a / 2) / *a;
			*g = (*g * 255 + *a / 2) / *a;
			*r = (*r * 255 + *a / 2) / *a;
		}
	}

	gfx_texture_upload(texture, texture->format, texture->data);
}

PangoLayout *gfx_text_layout_get_pango_layout(gfx_text_layout *textlayout)
{
	return textlayout ? textlayout->pango_layout : NULL;
}

void gfx_text_layout_set_foreground(gfx_text_layout *textlayout, const float red, const float green, const float blue, const float alpha)
{
	if(!textlayout)
		return;

	textlayout->foreground_color.red = red;
	textlayout->foreground_color.green = green;
	textlayout->foreground_color.blue = blue;
	textlayout->foreground_color.alpha = alpha;
}

void gfx_text_layout_set_background(gfx_text_layout *textlayout, float red, float green, float blue, float alpha)
{
	if(!textlayout)
		return;

	textlayout->background_color.red = red;
	textlayout->background_color.green = green;
	textlayout->background_color.blue = blue;
	textlayout->background_color.alpha = alpha;
}
