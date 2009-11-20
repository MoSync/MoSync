#ifndef MKDIR_H
#define MKDIR_H

#ifdef WIN32
#include <direct.h>
#elif defined(LINUX)
#include <sys/stat.h>
int _mkdir(const char* name);
inline int _mkdir(const char* name) {
	return mkdir(name, 0755);
}
#else
#error Unsupported platform
#endif

#endif	//MKDIR_H
