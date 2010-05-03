#include <windows.h>
#include "dgl.h"

void *_dgles_platform_open()
{
	HINSTANCE lib;

	lib = LoadLibrary(L"opengl32.dll");
	if (!lib)
	{
		lib = LoadLibrary(L"opengl.dll");
		if (!lib)
			return NULL;
	}

	return lib;
}

void *_dgles_platform_sym(void *handle, const char *name)
{
	return GetProcAddress((HINSTANCE) handle, name);
}

void _dgles_platform_close(void *h)
{
	FreeLibrary(h);
}
