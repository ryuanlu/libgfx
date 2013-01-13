#ifndef GFX_H_
#define GFX_H_

#include <pango/pangocairo.h>

typedef enum
{
	GFX_SUCCESS = 0,
	GFX_ERROR = -1,
	GFX_NUMBER_OF_RESULT
}gfx_result;

typedef enum
{
	GFX_PIXELFORMAT_RGBA32 = 0,
	GFX_PIXELFORMAT_RGB24,
	GFX_PIXELFORMAT_RG16,
	GFX_PIXELFORMAT_R8,
	GFX_PIXELFORMAT_BGRA32,
	GFX_PIXELFORMAT_BGR24,
	GFX_PIXELFORMAT_DEPTH32F,
	GFX_PIXELFORMAT_DEPTH32,
	GFX_PIXELFORMAT_DEPTH16,
	GFX_PIXELFORMAT_DEPTH8,
	GFX_NUMBER_OF_PIXELFORMAT
}gfx_pixel_format;

typedef enum
{
	GFX_ATTACH_COLOR_BUFFER = 0,
	GFX_ATTACH_DEPTH_BUFFER,
	GFX_NUMBER_OF_ATTACH_DEPTH_BUFFER
}gfx_fb_attachment;

typedef enum
{
	GFX_CLEAR_COLOR_BUFFER = (0x1 << 0),
	GFX_CLEAR_DEPTH_BUFFER = (0x1 << 1),
	GFX_CLEAR_COLOR_AND_DEPTH = GFX_CLEAR_COLOR_BUFFER|GFX_CLEAR_DEPTH_BUFFER
}gfx_fb_clear_target;

typedef enum
{
	GFX_VERTEX_SHADER = 0,
	GFX_FRAGMENT_SHADER,
	GFX_GEOMETRY_SHADER,
	GFX_NUMBER_OF_SHADER_TYPES
}gfx_shader_type;

typedef struct gfx_context gfx_context;
typedef struct gfx_texture gfx_texture;
typedef struct gfx_text_layout gfx_text_layout;
typedef struct gfx_framebuffer gfx_framebuffer;
typedef struct gfx_shader gfx_shader;
typedef struct gfx_program gfx_program;

void gfx_init(int *argc, char ***argv);

gfx_context *gfx_context_new(const gfx_context *sharelist);
gfx_result gfx_context_delete(gfx_context **context);
gfx_result gfx_context_make_current(gfx_context *context);

gfx_texture *gfx_texture_new(const int width, const int height, const int depth, const gfx_pixel_format format, const unsigned char *data);
gfx_result gfx_texture_delete(gfx_texture **texture);
gfx_result gfx_texture_generate_mipmaps(gfx_texture *texture);
gfx_result gfx_texture_bind(const int texture_unit, const gfx_texture *texture);
gfx_result gfx_texture_copy_from_framebuffer(gfx_texture *texture, const gfx_fb_attachment target);
cairo_t *gfx_texture_get_cairo_context(gfx_texture *texture);

gfx_text_layout *gfx_text_layout_new(const int width, const int height);
void gfx_text_layout_delete(gfx_text_layout **textlayout);
PangoLayout *gfx_text_layout_get_pango_layout(gfx_text_layout *textlayout);
void gfx_text_layout_set_foreground(gfx_text_layout *textlayout, const float red, const float green, const float blue, const float alpha);
void gfx_text_layout_set_background(gfx_text_layout *textlayout, const float red, const float green, const float blue, const float alpha);
void gfx_text_layout_render(gfx_text_layout *textlayout);
#define gfx_text_layout_set_alignment(textlayout,alignment) pango_layout_set_alignment(gfx_text_layout_get_pango_layout(textlayout),alignment)
#define gfx_text_layout_set_line_spacing(textlayout, space) pango_layout_set_spacing(gfx_text_layout_get_pango_layout(textlayout), space * PANGO_SCALE)
#define gfx_text_layout_set_indent(textlayout, indent) pango_layout_set_indent(gfx_text_layout_get_pango_layout(textlayout), indent * PANGO_SCALE)
#define gfx_text_layout_set_pango_markup(textlayout, markup) pango_layout_set_markup(gfx_text_layout_get_pango_layout(textlayout), markup, -1)

gfx_framebuffer *gfx_framebuffer_new(const int width, const int height, const gfx_pixel_format format, const int multisample, const int samples);
gfx_result gfx_framebuffer_delete(gfx_framebuffer **framebuffer);
gfx_result gfx_framebuffer_attach_texture(const gfx_fb_attachment target, gfx_texture *texture);
gfx_result gfx_framebuffer_bind(gfx_framebuffer *framebuffer);
void gfx_framebuffer_clear(const float red, const float green, const float blue, const float alpha, const gfx_fb_clear_target target);

#ifdef GFX_GLX_BUILD
#include <X11/Xlib.h>
void gfx_glx_blit_framebuffer(const gfx_framebuffer *framebuffer, const Window window);
#endif

gfx_shader *gfx_shader_new(const gfx_shader_type type, const char *source);
void gfx_shader_delete(gfx_shader** shader);
gfx_shader *gfx_shader_new_from_file(const gfx_shader_type type, const char *filename);
gfx_program *gfx_program_new(gfx_shader *vertex_shader, gfx_shader *geometry_shader, gfx_shader *fragment_shader);
void gfx_program_delete(gfx_program** program);


#endif /* GFX_H_ */
