/*
 * __isinfd(x) returns 1 if x is infinity, else 0;
 * no branching!
 * Added by Cygnus Support.
 */

#include "fdlibm.h"

#ifndef _DOUBLE_IS_32BITS

int
_DEFUN (__isinfd, (x),
	double x)
{
	__int32_t hx,lx;
	EXTRACT_WORDS(hx,lx,x);
	lx |= (hx & 0x7fffffff) ^ 0x7ff00000;
	lx |= -lx;
	return ~(lx >> 31) & (hx >> 30);
}

#endif /* _DOUBLE_IS_32BITS */
