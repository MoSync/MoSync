
#ifndef OSCL_MEM_H_INCLUDED
#define OSCL_MEM_H_INCLUDED

#include <string.h>

#ifndef DARWIN
#include <malloc.h>
#else
#include <stdlib.h>
#endif


#define oscl_memset memset
#define oscl_memcpy memcpy
#define oscl_memmove memmove
#define oscl_memcmp memcmp
#define oscl_malloc malloc
#define oscl_free free

#define OSCL_ARRAY_DELETE(ptr) delete [] ptr
#define OSCL_ARRAY_NEW(T, count) new T[count]

#endif	//OSCL_MEM_H_INCLUDED
