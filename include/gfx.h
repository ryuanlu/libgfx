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

typedef struct gfx_context* gfx_context;
typedef struct gfx_image* gfx_image;
typedef struct gfx_texture* gfx_texture;
typedef struct gfx_framebuffer* gfx_framebuffer;

void gfx_init(int *argc, char ***argv);

gfx_context gfx_context_new(const gfx_context sharelist);
gfx_result gfx_context_delete(gfx_context* context);
gfx_result gfx_context_make_current(const gfx_context context);

gfx_image gfx_image_new(const int width, const int height, const int depth, const gfx_pixel_format format, unsigned char* data);
gfx_result gfx_image_delete(gfx_image* image);
gfx_result gfx_image_copy_from_memory(gfx_image image, const char* data);
gfx_result gfx_image_copy_from_texture(gfx_image image, const gfx_texture texture);

gfx_texture gfx_texture_new(const int width, const int height, const int depth, const gfx_pixel_format format, const unsigned char* data);
gfx_result gfx_texture_delete(gfx_texture* texture);
gfx_texture gfx_texture_new_from_image(const gfx_image image);
gfx_result gfx_texture_copy_from_image(gfx_texture texture, const gfx_image image, const int offset_x, const int offset_y, const int offset_z);
gfx_result gfx_texture_generate_mipmaps(gfx_texture texture);
gfx_result gfx_texture_bind(const int texture_unit, const gfx_texture texture);
gfx_result gfx_texture_copy_from_framebuffer(gfx_texture texture, const gfx_framebuffer framebuffer, const gfx_fb_attachment target);

gfx_framebuffer gfx_framebuffer_new(const int width, const int height, const gfx_pixel_format format, const int multisample, const int samples);
gfx_result gfx_framebuffer_attach_texture(gfx_framebuffer framebuffer, const gfx_fb_attachment target, const gfx_texture texture);
gfx_result gfx_framebuffer_bind(gfx_framebuffer framebuffer);

#endif /* GFX_H_ */
