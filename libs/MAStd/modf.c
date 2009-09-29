/*	$MoSync: modf.c 2007/08/22 Fredrik	*/
/*This code was originally used on the PowerPC architecture.*/

/*	$OpenBSD: modf.c,v 1.8 2005/08/07 16:40:15 espie Exp $	*/
/* @(#)s_modf.c 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/*
 * modf(double x, double *iptr) 
 * return fraction part of x, and return x's integral part in *iptr.
 * Method:
 *	Bit twiddling.
 *
 * Exception:
 *	No exception.
 */

#ifndef MAPIP
#error modf is defined on all architectures already; you do not need to compile this file.
#endif

#include "madmath.h"

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/*
 * from: @(#)fdlibm.h 5.1 93/09/24
 */

//#include <sys/types.h>
//#include <machine/endian.h>
typedef unsigned int u_int32_t;
typedef int int32_t;

/* The original fdlibm code used statements like:
	n0 = ((*(int*)&one)>>29)^1;		* index of high word *
	ix0 = *(n0+(int*)&x);			* high word of x *
	ix1 = *((1-n0)+(int*)&x);		* low word of x *
   to dig two 32 bit words out of the 64 bit IEEE floating point
   value.  That is non-ANSI, and, moreover, the gcc instruction
   scheduler gets it wrong.  We instead use the following macros.
   Unlike the original code, we determine the endianness at compile
   time, not at run time; I don't see much benefit to selecting
   endianness at run time.  */

/* A union which permits us to convert between a double and two 32 bit
   ints.  */

/*
 * The arm32 port is little endian except for the FP word order which is
 * big endian.
 */

#if 0
typedef union 
{
	double value;
	struct 
	{
		u_int32_t msw;
		u_int32_t lsw;
	} parts;
} ieee_double_shape_type;
#else
typedef union 
{
	double value;
	struct 
	{
		u_int32_t lsw;
		u_int32_t msw;
	} parts;
} ieee_double_shape_type;
#endif

/* Get two 32 bit ints from a double.  */

#define EXTRACT_WORDS(ix0,ix1,d)				\
	do {								\
	ieee_double_shape_type ew_u;					\
	ew_u.value = (d);						\
	(ix0) = ew_u.parts.msw;					\
	(ix1) = ew_u.parts.lsw;					\
	} while (0)

/* Get the more significant 32 bit int from a double.  */

#define GET_HIGH_WORD(i,d)					\
	do {								\
	ieee_double_shape_type gh_u;					\
	gh_u.value = (d);						\
	(i) = gh_u.parts.msw;						\
	} while (0)

/* Get the less significant 32 bit int from a double.  */

#define GET_LOW_WORD(i,d)					\
	do {								\
	ieee_double_shape_type gl_u;					\
	gl_u.value = (d);						\
	(i) = gl_u.parts.lsw;						\
	} while (0)

/* Set a double from two 32 bit ints.  */

#define INSERT_WORDS(d,ix0,ix1)					\
	do {								\
	ieee_double_shape_type iw_u;					\
	iw_u.parts.msw = (ix0);					\
	iw_u.parts.lsw = (ix1);					\
	(d) = iw_u.value;						\
	} while (0)

/* Set the more significant 32 bits of a double from an int.  */

#define SET_HIGH_WORD(d,v)					\
	do {								\
	ieee_double_shape_type sh_u;					\
	sh_u.value = (d);						\
	sh_u.parts.msw = (v);						\
	(d) = sh_u.value;						\
	} while (0)

/* Set the less significant 32 bits of a double from an int.  */

#define SET_LOW_WORD(d,v)					\
	do {								\
	ieee_double_shape_type sl_u;					\
	sl_u.value = (d);						\
	sl_u.parts.lsw = (v);						\
	(d) = sl_u.value;						\
	} while (0)


static const double one = 1.0;

double modf(double x, double *iptr)
{
	int32_t i0,i1,j0;
	u_int32_t i;
	EXTRACT_WORDS(i0,i1,x);
	j0 = ((i0>>20)&0x7ff)-0x3ff;	/* exponent of x */
	if(j0<20) {			/* integer part in high x */
		if(j0<0) {			/* |x|<1 */
			INSERT_WORDS(*iptr,i0&0x80000000,0);	/* *iptr = +-0 */
			return x;
		} else {
			i = (0x000fffff)>>j0;
			if(((i0&i)|i1)==0) {		/* x is integral */
				u_int32_t high;
				*iptr = x;
				GET_HIGH_WORD(high,x);
				INSERT_WORDS(x,high&0x80000000,0);	/* return +-0 */
				return x;
			} else {
				INSERT_WORDS(*iptr,i0&(~i),0);
				return x - *iptr;
			}
		}
	} else if (j0>51) {		/* no fraction part */
		u_int32_t high;
		*iptr = x*one;
		GET_HIGH_WORD(high,x);
		INSERT_WORDS(x,high&0x80000000,0);	/* return +-0 */
		return x;
	} else {			/* fraction part in low x */
		i = ((u_int32_t)(0xffffffff))>>(j0-20);
		if((i1&i)==0) { 		/* x is integral */
			u_int32_t high;
			*iptr = x;
			GET_HIGH_WORD(high,x);
			INSERT_WORDS(x,high&0x80000000,0);	/* return +-0 */
			return x;
		} else {
			INSERT_WORDS(*iptr,i0,i1&(~i));
			return x - *iptr;
		}
	}
}

