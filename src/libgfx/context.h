#ifndef CONTEXT_H_
#define CONTEXT_H_

#ifdef GFX_GLX_BUILD
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif
#include "framebuffer.h"

struct gfx_context
{
#ifdef GFX_GLX_BUILD
	Display *display;
	GLXContext opengl_context;
#endif
	gfx_framebuffer *current_framebuffer;

};

#pragma GCC visibility push(hidden)
extern gfx_context *gfx_current_context;
#pragma GCC visibility pop

#endif /* CONTEXT_H_ */
