#ifndef GFX_H_
#define GFX_H_

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

typedef struct gfx_context* gfx_context;
typedef struct gfx_texture* gfx_texture;
typedef struct gfx_framebuffer* gfx_framebuffer;

void gfx_init(int *argc, char ***argv);

gfx_context gfx_context_new(const gfx_context sharelist);
gfx_result gfx_context_delete(gfx_context* context);
gfx_result gfx_context_make_current(const gfx_context context);

gfx_texture gfx_texture_new(const int width, const int height, const int depth, const gfx_pixel_format format, const unsigned char* data);
gfx_result gfx_texture_delete(gfx_texture* texture);
gfx_result gfx_texture_generate_mipmaps(gfx_texture texture);
gfx_result gfx_texture_bind(const int texture_unit, const gfx_texture texture);
gfx_result gfx_texture_copy_from_framebuffer(gfx_texture texture, const gfx_fb_attachment target);
void gfx_texture_draw_pango_markup(gfx_texture texture, const int x, const int y, const int width, const int wrapping, const char* markup);

gfx_framebuffer gfx_framebuffer_new(const int width, const int height, const gfx_pixel_format format, const int multisample, const int samples);
gfx_result gfx_framebuffer_delete(gfx_framebuffer* framebuffer);
gfx_result gfx_framebuffer_attach_texture(const gfx_fb_attachment target, const gfx_texture texture);
gfx_result gfx_framebuffer_bind(const gfx_framebuffer framebuffer);
void gfx_framebuffer_clear(const float red, const float green, const float blue, const float alpha, const gfx_fb_clear_target target);

#ifdef GFX_GLX_BUILD
#include <X11/Xlib.h>
void gfx_glx_blit_framebuffer(const gfx_framebuffer framebuffer, const Window window);
#endif


#endif /* GFX_H_ */
