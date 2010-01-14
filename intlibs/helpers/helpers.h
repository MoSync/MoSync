/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#ifndef HELPERS_H
#define HELPERS_H

#ifdef __SYMBIAN32__
#include "my_stdlib.h"
#else
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#endif

#include "cpp_defs.h"
#include "types.h"
#include "log.h"

#ifndef _SE_MSAB_MASTD_MAASSERT_H_
#define _SE_MSAB_MASTD_MAASSERT_H_
#define MAASSERT DEBUG_ASSERT
#endif


//******************************************************************************
//Function declarations
//******************************************************************************

//should exit immediately
#if defined(_MSC_VER) || defined(__SYMBIAN32__)
void __declspec(noreturn) MoSyncExit(int code);
#elif defined(__GNUC__)
void __attribute((noreturn)) MoSyncExit(int code);
#else
#error Unsupported platform!
#endif

//should tell the user something bad happened, then exit.
#if defined(_MSC_VER) || defined(__SYMBIAN32__)
void __declspec(noreturn) MoSyncErrorExit(int errorCode);
#elif defined(__GNUC__)
void __attribute((noreturn)) MoSyncErrorExit(int errorCode);
#endif

#ifndef __SYMBIAN32__
inline void lower(std::string& str) {
	for(size_t i=0; i<str.length(); i++) {
		str[i] = (char)tolower(str[i]);
	}
}
#endif

//******************************************************************************
//Renamed functions
//******************************************************************************

#ifndef _MSC_VER
#define _snprintf snprintf
#endif

//******************************************************************************
//Inline functions
//******************************************************************************

inline bool isPowerOf2(unsigned int x) {
	return !(x & (x-1));
}

//returns the smallest power of 2 that is >= 2^min and >= x.
inline uint nextPowerOf2(uint minPow, uint x) {
	uint i = 1 << minPow;
	while(i < x) {
		i <<= 1;		
	}
	return i;
}

//returns the biggest power of 2 that is >= 2^min and <= max
inline uint biggestPowerOf2(uint minPow, uint max) {
	uint i = 1 << minPow;
	while(i <= max) {
		i <<= 1;		
	}
	i >>= 1;
	return i;
}

template<class T> void swap(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}

/*__inline const char* FileNameFromPath(const char* path) {
	const char* ptr = strrchr(path, '\\');
	return ptr ? (ptr + 1) : path;
}*/

//******************************************************************************
//Error definitions
//******************************************************************************
#define ERROR_HELPERS_PREFIX 0
#define ERROR_BASE_PREFIX 40000
#define ERROR_SDL_PREFIX 80000
#define ERROR_BT_PREFIX 120000
#define ERROR_WINDOWS_PREFIX 160000
#define ERROR_SYMBIAN_PREFIX 200000
#define ERROR_WCE_PREFIX 240000
#define ERROR_NET_PREFIX 280000

#define HELPERS_ERRORS(m)\
	m(1, ERR_INTERNAL, "Internal error")\
	m(2, ERR_OOM, "Out of memory")\
	m(3, ERR_FIFO_OVERRUN, "FIFO overrun")\
	m(4, ERR_FUNCTION_UNIMPLEMENTED, "Function is not yet implemented on this platform")\

#define COMMA_FIRST(f, s) ,f
#define COMMA_SECOND_DOT(f, s) ,s "."

#define ERROR_ENUM_SINGLE(val, id, desc) ,id = val

#define DECLARE_ERROR_ENUM(set) namespace MoSyncError {\
	enum { ERROR_##set##_BASE = ERROR_##set##_PREFIX\
	set##_ERRORS(ERROR_ENUM_SINGLE), ERROR_##set##_END };\
	const char* set##_translate(int code); }

DECLARE_ERROR_ENUM(HELPERS)

//Must be implemented by each platform. Use errors_inline.h.
namespace MoSyncError {
	const char* subSystemString(int code);
	const char* errorDescription(int code);
}


//******************************************************************************
//Macros
//******************************************************************************

#define INVALID_SYSCALL_NUMBER 256
#define SYSCALL_NUMBER_IS_VALID(n) ((n) < INVALID_SYSCALL_NUMBER && (n) >= 0)

#define CBP (const byte*)
#define CCP (const char*)

#define SAFE_DELETE(ptr) { if(ptr) { delete (ptr); (ptr) = NULL; } }

#ifdef BIG_PHAT_ERROR
#undef BIG_PHAT_ERROR
#endif
#define BIG_PHAT_ERROR(errorCode) { LOG("BFE "); IN_FILE_ON_LINE; MoSyncErrorExit(errorCode); }

#ifdef _DEBUG
#define DEBUG_BREAK if(IsDebuggerPresent()) DebugBreak()
#define FAIL_FUNCTION failFunction()
#else
#define DEBUG_BREAK
#define FAIL_FUNCTION
#endif

#define MYASSERT(a, errorCode) MYASSERT_EXTRA(a, errorCode, )
#define MYASSERT_EXTRA(a, errorCode, extra) if(!(a)) { LOG("Assert failure %s ", #a);\
	IN_FILE_ON_LINE; extra; MoSyncErrorExit(errorCode); }

#define DEBUG_ASSERT(a) MYASSERT(a, MoSyncError::ERR_INTERNAL)
#define DEBIG_PHAT_ERROR BIG_PHAT_ERROR(MoSyncError::ERR_INTERNAL)

#define IN_FILE_ON_LINE LOG("in %s @ line %i\n", __FILE__, __LINE__)
#define FAIL { LOG("Failure "); IN_FILE_ON_LINE; FAIL_FUNCTION; return 0; }
#define LOG_VAL(a) { LOG("Failure %i ", a); IN_FILE_ON_LINE; }
#define FAIL_VAL(val) { LOG_VAL(val); FAIL_FUNCTION; return false; }
#define FAILIF(b) if(b) FAIL
#define TEST TEST_Z
#define TEST_Z(a) FAILIF((a) == 0)
#define TEST_NEQ(a, b) { int ta=(a); int tb=(b); if(ta != tb) { LOG("%i != %i; ", ta, tb); FAIL } }
#define TEST_LTZ(a) FAILIF((a) < 0)
#define TEST_LEZ(a) FAILIF((a) <= 0)
#define TEST_NZ(a) TEST_NEQ(a, 0)

#define TLTZ_PASS(a) { int ta=(a); if(ta < 0) { LOG("Failure "); IN_FILE_ON_LINE; FAIL_FUNCTION; return ta; } }

#define CHECK_NZ(a) { int test_s = (a); if(test_s != 0) LOG_VAL(test_s); }

#define GLECUSTOM(a) if((a)) { LOG("GLE %i, ", (int)GetLastError()); BIG_PHAT_ERROR(ERR_INTERNAL); }
#define GLE(a) GLECUSTOM((a) == 0)

#define HRES(a) { HRESULT hres = (a); if(FAILED(hres)) { LOG("HRES 0x%08X, ", hres);\
	BIG_PHAT_ERROR(ERR_INTERNAL); } }

#define ASRTINT(func, i, errorCode) { int temp = (func); if(temp != (i)) {\
	LOG("Failure, not %i: %s = %i\n", i, #func, temp); IN_FILE_ON_LINE; MoSyncErrorExit(errorCode); } }
#define DEBUG_ASRTINT(func, i) ASRTINT(func, i, ERR_INTERNAL)
#define ASRTZERO(func, errorCode) ASRTINT(func, 0, errorCode)
#define DEBUG_ASRTZERO(func) ASRTINT(func, 0, ERR_INTERNAL)

#ifdef MEMORY_DEBUG
#define CHECK_INT_ALIGNMENT(ptr) MYASSERT(((int)(ptr) & 0x3) == 0, ERR_MEMORY_ALIGNMENT)
#else
#define CHECK_INT_ALIGNMENT(ptr)
#endif
#define ALIGN_INT(ptr) ((int*)((int)(ptr) & ~0x3))

//Test and Return Minus 1
#define TRM1(func) if(!(func)) { LOG("Failure "); IN_FILE_ON_LINE; failFunction(); return -1; }

#ifdef _MSC_VER	//Microsoft C
#define INT64PREFIX "I64"
#else	//glibc-compatible
#define INT64PREFIX "ll"
#endif

#define DUMPINT(i) LOG("%s: %i\n", #i, i)
#define DUMPINT64(i) LOG("%s: %" INT64PREFIX "i\n", #i, i)
#define DUMPHEX(i) LOG("%s: 0x%x\n", #i, i)

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
#define sgn(x)	((x) > 0 ? 1: ((x) == 0 ? 0 : (-1)))
//#ifdef _MSC_VER
//#define MAKE(type, a) (*(type*)&(a))
//#else
template<class Dst, class Src> Dst union_cast(Src s) {
	union {
		Dst d;
		Src s;
	} u;
	u.s = s;
	return u.d;
}
#define MAKE(type, a) union_cast<type>(a)
//#endif

#define COMMA ,
#define NUL
#define NULA(a)
#define NULA2(a,b)

#define ZERO_ARRAY(a) memset(a, 0, sizeof(a))
#define ZERO_OBJECT(a) memset(&a, 0, sizeof(a))
#define ZEROMEM(a, size) memset(a, 0, size)

#ifndef NULL
#define NULL 0
#endif

#define sstrcmp(charP, literal) strncmp(charP, literal, sizeof(literal)-1)

#endif	//HELPERS_H
