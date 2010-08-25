#include "SDL.h"
#include <GLES/gl.h>
#include <stdio.h>
#include <stdlib.h>

extern int init(void);
extern int test(void);
GL_API int GL_APIENTRY _dgles_load_library(void *, void *(*)(void *, const char *));

static void *proc_loader(void *h, const char *name)
{
	(void) h;
	return SDL_GL_GetProcAddress(name);
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0)
	{
		fprintf(stderr, "SDL error: %s\n", SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);

	{
		int err;
#if defined(WIN32)
		err = SDL_GL_LoadLibrary("opengl32.dll");
#elif defined(__linux__) || defined(__FreeBSD__)
		err = SDL_GL_LoadLibrary("libGL.so");
#else
#error Your platform is not supported
		err = 1;
#endif
		if (err != 0)
		{
			fprintf(stderr, "Can't load OpenGL library: %s\n", SDL_GetError());
			return 1;
		}
	}

	SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 0);

	if (SDL_SetVideoMode(600, 600, 0, SDL_OPENGL) == NULL)
	{
		fprintf(stderr, "SDL error: %s\n", SDL_GetError());
		return 1;
	}

	_dgles_load_library(NULL, proc_loader);

	SDL_WM_SetCaption("Desktop OpenGL ES", NULL);

	return test();
}

void frame_done()
{
	SDL_Event ev;

	glFlush();
	glFinish();
	SDL_GL_SwapBuffers();
	
	SDL_Delay(1);
	
	while (SDL_PollEvent(&ev))
	{
		if (ev.type == SDL_QUIT)
			exit(0);
	}
}
