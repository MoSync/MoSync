#ifndef STDDEF_H
#define STDDEF_H

#include <newlib.h>

typedef int wint_t;
typedef unsigned long size_t;
typedef int ptrdiff_t;

#ifndef _WCHAR_DEFINED
#define _WCHAR_DEFINED
#ifdef __cplusplus
typedef wchar_t wchar;
#else
typedef unsigned short wchar;
typedef wchar wchar_t;
#endif	//__cplusplus
#endif	//_WCHAR_DEFINED

#if __GNUC__ >= 4
#define _ATTRIBUTE_WARNING(x) __attribute__((warning(x)))
#else
#define _ATTRIBUTE_WARNING(x)
#endif	//__GNUC__

#ifndef NULL
#define NULL 0
#endif

#ifndef __cplusplus
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#else
/* The cast to "char &" below avoids problems with user-defined
   "operator &", which can appear in a POD type.  */
#define offsetof(TYPE, MEMBER)					\
  (__offsetof__ (reinterpret_cast <size_t>			\
                 (&reinterpret_cast <const volatile char &>	\
                  (static_cast<TYPE *> (0)->MEMBER))))
#endif /* C++ */

#endif	//STDDEF_H
