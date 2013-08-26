/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#if 0
#include "ma.h"

// Functions are intrinsic, no prototypes needed.
// See http://gcc.gnu.org/onlinedocs/gccint/Integer-library-routines.html
// for documentation.

typedef struct
{
	unsigned int lo, hi;
} int64;

#define JRI_BIT(n)			((unsigned int)1 << (n))
#define JRI_BITMASK(n)		(JRI_BIT(n) - 1)
#define _jlong_lo16(a)		((a) & JRI_BITMASK(16))
#define _jlong_hi16(a)		((a) >> 16)

#define int64_MUL32(r, a, b) {\
     unsigned int _a1, _a0, _b1, _b0, _y0, _y1, _y2, _y3;\
     _a1 = _jlong_hi16(a), _a0 = _jlong_lo16(a);\
     _b1 = _jlong_hi16(b), _b0 = _jlong_lo16(b);\
     _y0 = _a0 * _b0;\
     _y1 = _a0 * _b1;\
     _y2 = _a1 * _b0;\
     _y3 = _a1 * _b1;\
     _y1 += _jlong_hi16(_y0);                   /* can't carry */\
     _y1 += _y2;                                /* might carry */\
     if (_y1 < _y2) _y3 += 1 << 16;             /* propagate */\
     (r).lo = (_jlong_lo16(_y1) << 16) + _jlong_lo16(_y0);\
     (r).hi = _y3 + _jlong_hi16(_y1);\
}\

long long __muldi3(long long a, long long b)
{
	int64 r;
    int64 _a, _b;                                                   
    _a = *((int64*)&a); _b = *((int64*)&b);                                              
    int64_MUL32(r, _a.lo, _b.lo);                                       
    (r).hi += _a.hi * _b.lo + _a.lo * _b.hi;   
	return (long long)*((long long*)&r);
}

long long __negdi2(long long num) {
	return (~num)+1;
}

unsigned long long libgcc_udivmoddi4(unsigned long long num, unsigned long long den, unsigned long long *rem_p)
{
  unsigned long long quot = 0, qbit = 1;

  if ( den == 0 ) {
    maPanic(0, "division by zero");
    return 0;			/* If trap returns... */
  }

  /* Left-justify denominator and count shift */
  while ( (long long)den >= 0 ) {
    den <<= 1;
    qbit <<= 1;
  }

  while ( qbit ) {
    if ( den <= num ) {
      num -= den;
      quot += qbit;
    }
    den >>= 1;
    qbit >>= 1;
  }

  if ( rem_p )
    *rem_p = num;

  return quot;
}

long long libgcc_divdi3(long long num, long long den)
{
  int minus = 0;
  long long v;

  if ( num < 0 ) {
    //num = -num;
	num = (~num)+1;
    minus = 1;
  }
  if ( den < 0 ) {
    //den = -den;
	den = (~den)+1;

    minus ^= 1;
  }

  v = libgcc_udivmoddi4(num, den, NULL);
  if ( minus )
  //  v = -v;
	v = (~v)+1;


  return v;
}

long long __divdi3(long long a, long long b)
{
	return libgcc_divdi3(a, b);
}

unsigned long long __udivdi3 (unsigned long long a, unsigned long long b)
{
	return libgcc_udivmoddi4(a, b, NULL);
}

unsigned long long __umoddi3 (unsigned long long a, unsigned long long b)
{
	unsigned long long rem;
	libgcc_udivmoddi4(a, b, &rem);
	return rem;
}

#if 0
long long __ashrdi3(long long a, int b)
{
	maPanic(0, "__ashrdi3 unimplemented");
	return 0;
}

long long __ashldi3(long long a, int b)
{
	maPanic(0, "__ashldi3 unimplemented");
	return 0;
}
#endif	//0
#endif
