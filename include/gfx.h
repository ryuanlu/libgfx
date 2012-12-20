#ifndef GFX_H_
#define GFX_H_


typedef enum
{
	GFX_SUCCESS = 0,
	GFX_ERROR = -1,

}gfx_result;

typedef struct gfx_context* gfx_context;


void gfx_init(int *argc, char ***argv);
gfx_context gfx_context_new(gfx_context sharelist);
gfx_result gfx_context_delete(gfx_context* context);
gfx_result gfx_context_make_current(gfx_context context);

#endif /* GFX_H_ */
