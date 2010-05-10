/*
 * common.h
 *
 *  Created on: 2009-jun-09
 *      Author: Ali Mosavian
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#ifndef NULL
#define NULL 0
#endif

#ifndef EXIT_FAILURE
#define EXIT_FAILURE -1
#endif

typedef signed char		sint8;
typedef signed short	sint16;
typedef signed int		sint32;
typedef signed long long		sint64;
typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned int	uint32;
typedef unsigned long long	uint64;
typedef float			float32;
typedef double			float64;
typedef sint32			fix32;

/* Fixed point macros */
#define FRAC_BITS			16
#define FIX_ONE				(1<<FRAC_BITS)
#define FIX_HALF			(1<<FRAC_BITS-1)
#define INT2FIX(x) 			(fix32)((x)<<FRAC_BITS)
#define FLT2FIX(x) 			(fix32)((x)*((float32)FIX_ONE)+0.5f)
#define FIX2FLT(x) 			(float)((x)/((float32)FIX_ONE))
#define FIXROUND(x) 		(((x)+FIX_HALF) & (0xffffffff<<FRAC_BITS))
#define FIXFLOOR(x) 		( (x) & (0xffffffff<<FRAC_BITS))
#define FIX2INT_ROUND(x) 	(((x)+FIX_HALF) >> FRAC_BITS )
#define FIX2INT_FLOOR(x) 	((x) >> FRAC_BITS)
//#define FIXMULT(x, y) 		((x)*(y)>>FRAC_BITS)
#define FIXMULT(x, y) 		((x>>6)*(y>>6)>>(2*(FRAC_BITS-6)-FRAC_BITS)) //Truncates the fraction part to FRAC_BITS-6 bits
#define FIXMULT_EXT(x, y) 	((((sint64)(x))*(y))>>FRAC_BITS)
//#define FIXDIV(x, y) 		(((sint64)(x)<<FRAC_BITS)/(y))
#define FIXDIV(x, y) 		(((x<<6)/(y))<<(FRAC_BITS-6)) 	//Truncates the whole part to FRAC_BITS-6 bits
#define FIXDIV_EXT(x, y) 	(((sint64)(x)<<FRAC_BITS)/(y))


/**
 * Quits the application with a message
 *
 */
void FatalError ( char msg[] );


#endif /* __COMMON_H__ */

