/*
 * __isinff(x) returns 1 if x is +-infinity, else 0;
 * Added by Cygnus Support.
 */

#include "fdlibm.h"

int
_DEFUN (__isinff, (x),
	float x)
{
	__int32_t ix,t;
	GET_FLOAT_WORD(ix,x);
	t = ix & 0x7fffffff;
	t ^= 0x7f800000;
	t |= -t;
	return ~(t >> 31) & (ix >> 30);
}

#ifdef _DOUBLE_IS_32BITS

int
_DEFUN (__isinfd, (x),
	double x)
{
	return __isinff((float) x);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
