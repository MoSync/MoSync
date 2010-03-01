#ifndef MASTRING_H
#define MASTRING_H

#include <string.h>

inline static int memeq(const void* a, const void* b, int size) {
	return memcmp(a, b, size) == 0;
}

#endif	//MASTRING_H
