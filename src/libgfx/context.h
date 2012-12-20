#ifndef CONTEXT_H_
#define CONTEXT_H_

#ifdef GFX_GLX_BUILD
#include <X11/Xlib.h>
#include <GL/glx.h>
#endif

struct gfx_context
{
#ifdef GFX_GLX_BUILD
	Display* display;
	GLXContext opengl_context;
#endif
	GLuint fbo;

};


#endif /* CONTEXT_H_ */
