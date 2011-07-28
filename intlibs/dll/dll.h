#ifndef DLL_H
#define DLL_H

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

class Dll {
public:
	// sets up any needed variables.
	Dll();
	// calls close().
	~Dll();
	// returns false on fail.
	bool open(const char* fileName);
	// returns NULL on fail.
	void* get(const char* functionName);
	// cannot fail.
	void close();
private:
#ifdef WIN32
	HMODULE mHandle;
#else
	void* mHandle;
#endif
};

#endif	//DLL_H
