#include <stdio.h>
#include <stdlib.h>
#ifdef GFX_GLX_BUILD
#include <GL/glxew.h>
#endif
#include <GL/glew.h>
#include "gfx.h"
#include "context.h"


gfx_context *gfx_current_context = NULL;
static int glewInitialized = 0;

#ifdef GFX_GLX_BUILD
static void create_glx_render_context(GLXContext *context, Display **display, GLXContext sharelist)
{

	int visual_attr[] = {	GLX_RGBA,
				GLX_DOUBLEBUFFER,
				GLX_RED_SIZE, 8,
				GLX_GREEN_SIZE, 8,
				GLX_BLUE_SIZE, 8,
				GLX_DEPTH_SIZE, 24,
				0 };

	int version_attr[] = {	GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
				GLX_CONTEXT_MINOR_VERSION_ARB, 3, 0};
	int nitems;

	Window window;
	XVisualInfo *vi = NULL;
	GLXContext rc = NULL;
	GLXContext newrc = NULL;
	GLXFBConfig *fbconfig = NULL;

	*display = XOpenDisplay(NULL);
	window = XRootWindow(*display, 0);


	/* Create a legacy context and use it to initialize GLEW */

	vi = glXChooseVisual(*display, DefaultScreen(*display), visual_attr);
	rc = glXCreateContext(*display, vi, 0, GL_TRUE);
	glXMakeCurrent(*display, window, rc);

	if(!glewInitialized)
	{
		glewInit();
		glewInitialized = 1;
	}


	/* Create a new context by using GLX_ARB_create_context extension after GLEW is initialized */

	fbconfig = glXChooseFBConfig(*display, DefaultScreen(*display), NULL, &nitems);
	newrc = glXCreateContextAttribsARB(*display, *fbconfig, sharelist, 1, version_attr);

	if(newrc) glXDestroyContext(*display, rc);

	glXMakeCurrent(*display, None, NULL);
	*context = newrc;
}

static void destroy_glx_render_context(Display *display, GLXContext context)
{
	glXDestroyContext(display, context);
	XCloseDisplay(display);
}

void gfx_glx_blit_framebuffer(const gfx_framebuffer *framebuffer, const Window window)
{
	glXMakeCurrent(gfx_current_context->display, window, gfx_current_context->opengl_context);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer->fbo);

	glBlitFramebuffer(0, 0, framebuffer->width, framebuffer->height, 0, 0, framebuffer->width, framebuffer->height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gfx_current_context->current_framebuffer->fbo);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	glXSwapBuffers(gfx_current_context->display, window);
	gfx_context_make_current(gfx_current_context);
}

#endif

gfx_context *gfx_context_new(const gfx_context *sharelist)
{
	gfx_context *context = NULL;

	context = calloc(1, sizeof(struct gfx_context));

#ifdef GFX_GLX_BUILD
	{
		GLXContext rc = NULL;

		if(sharelist)
			rc = sharelist->opengl_context;

		create_glx_render_context(&context->opengl_context, &context->display, rc);
	}
#endif

	gfx_context_make_current(context);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Initialize some built-in shaders */

	return context;
}

gfx_result gfx_context_delete(gfx_context **context)
{
	if(!context||!*context) return GFX_ERROR;

#ifdef GFX_GLX_BUILD
	destroy_glx_render_context((*context)->display, (*context)->opengl_context);
#endif
	if(*context == gfx_current_context)
		gfx_current_context = NULL;
	free(*context);
	*context = NULL;

	return GFX_SUCCESS;
}


gfx_result gfx_context_make_current(gfx_context *context)
{
	Bool ret = True;

	if(!context)
	{
		if(gfx_current_context)
#ifdef GFX_GLX_BUILD
			ret = glXMakeCurrent(gfx_current_context->display, None, NULL);
#endif
	}

	if(gfx_current_context != context)
	{
#ifdef GFX_GLX_BUILD
		ret = glXMakeCurrent(context->display, XRootWindow(context->display, 0), context->opengl_context);
#endif
		if(ret)
			gfx_current_context = context;
	}

	return ret ? GFX_SUCCESS : GFX_ERROR;
}
