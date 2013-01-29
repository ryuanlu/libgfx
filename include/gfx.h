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
	GFX_IMAGE_FORMAT_BMP = 0,
	GFX_IMAGE_FORMAT_JPG,
	GFX_IMAGE_FORMAT_PNG,
	GFX_IMAGE_FORMAT_SVG,
	GFX_NUMBER_OF_IMAGE_FORMAT
}gfx_image_format;

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

typedef enum
{
	GFX_SIMPLE_COLOR_PROGRAM = 0,
	GFX_SIMPLE_TEXTURE_PROGRAM,
	GFX_FULL_FEATURED_PROGRAM,
	GFX_NUMBER_OF_PROGRAM_PROFILES
}gfx_program_profile;

typedef enum
{
	GFX_ATTRIBUTE_ARRAY = 0,
	GFX_INDEX_ARRAY,
	GFX_NUMBER_OF_ARRAY_TYPES
}gfx_array_type;

typedef enum
{
	GFX_BYTE = 0,
	GFX_UNSIGNED_BYTE,
	GFX_SHORT,
	GFX_UNSIGNED_SHORT,
	GFX_INT,
	GFX_UNSIGNED_INT,
	GFX_FLOAT,
	GFX_DOUBLE
}gfx_data_type;

typedef enum
{
	GFX_POINTS = 0,
	GFX_LINES,
	GFX_LINE_STRIP,
	GFX_LINE_LOOP,
	GFX_TRIANGLES,
	GFX_TRIANGLE_STRIP,
}gfx_draw_mode;

typedef enum
{
	GFX_STATIC_DRAW = 0,
	GFX_DYNAMIC_DRAW,
	GFX_STREAM_DRAW
}gfx_draw_hint;

typedef struct gfx_context gfx_context;
typedef struct gfx_texture gfx_texture;
typedef struct gfx_text_layout gfx_text_layout;
typedef struct gfx_framebuffer gfx_framebuffer;
typedef struct gfx_shader gfx_shader;
typedef struct gfx_program gfx_program;
typedef float mat4[16];
typedef struct gfx_vertex_attribute gfx_vertex_attribute;
typedef struct gfx_vertex_array gfx_vertex_array;

void gfx_init(int *argc, char ***argv);

gfx_context *gfx_context_new(const gfx_context *sharelist);
gfx_result gfx_context_delete(gfx_context **context);
gfx_result gfx_context_make_current(gfx_context *context);

gfx_texture *gfx_texture_new(const int width, const int height, const int depth, const gfx_pixel_format format, const unsigned char *data);
gfx_result gfx_texture_delete(gfx_texture **texture);
gfx_texture *gfx_texture_new_from_source(const gfx_image_format format, char *data, const int length);
gfx_texture *gfx_texture_new_from_file(const gfx_image_format format, const char *filename);
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

gfx_shader *gfx_shader_new(const gfx_shader_type type, const char *source, const int size);
void gfx_shader_delete(gfx_shader **shader);
gfx_shader *gfx_shader_new_from_file(const gfx_shader_type type, const char *filename);
gfx_program *gfx_program_new(gfx_shader *vertex_shader, gfx_shader *geometry_shader, gfx_shader *fragment_shader);
void gfx_program_delete(gfx_program **program);
void gfx_program_use(const gfx_program *program);
gfx_program *gfx_program_new_from_integrated(const gfx_program_profile profile);
void gfx_program_set_uniform_mat4(const char *uniform, mat4 matrix);

void gfx_mat4_set_identity(mat4 matrix);
void gfx_mat4_multiply(mat4 output, const mat4 A, const mat4 B);
void gfx_mat4_set_translate(mat4 matrix, const float x, const float y, const float z);
void gfx_mat4_set_rotate(mat4 matrix, const float x, const float y, const float z, const float angle);
void gfx_mat4_set_scale(mat4 matrix, const float x, const float y, const float z);
void gfx_mat4_set_lookat(mat4 matrix, const float eyeX, const float eyeY, const float eyeZ, const float centerX, const float centerY, const float centerZ, const float upX, const float upY, const float upZ);
void gfx_mat4_set_frustrum(mat4 matrix, const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal);
void gfx_mat4_set_perspective(mat4 matrix, const float fovy, const float aspect, const float zNear, const float zFar);
void gfx_mat4_set_ortho(mat4 matrix, const float left, const float right, const float bottom, const float top, const float nearVal, const float farVal);

gfx_vertex_attribute *gfx_vertex_attribute_new(const gfx_array_type array_type, void *data, const gfx_data_type data_type, const int count, const gfx_draw_hint hint);
gfx_vertex_array *gfx_vertex_array_new(void);
void gfx_vertex_array_bind(gfx_vertex_array *vertex_array);
void gfx_vertex_array_attach_attribute(const gfx_vertex_attribute *attribute, const int index, const int size, const int type, const int normalized, const int stride, const void *pointer);
void gfx_vertex_array_attach_index(const gfx_vertex_attribute *attribute);
void gfx_vertex_array_draw(const gfx_draw_mode mode, const int start, const int count);


#endif /* GFX_H_ */
