#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "dgl.h"

#ifdef __APPLE__
#define OPENGL_LIBRARY "/System/Library/Frameworks/OpenGL.framework/Libraries/libGL.dylib"
#else
#define OPENGL_LIBRARY "libGL.so"
#endif

void *_dgles_platform_open(void)
{
	return dlopen(OPENGL_LIBRARY, RTLD_NOW);
}

void _dgles_platform_close(void *h)
{
	dlclose(h);
}

void *_dgles_platform_sym(void *handle, const char *name)
{
	return dlsym(handle, name);
}
