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

#include "config_platform.h"

#ifdef PHONE_RELEASE
#define DEBUG_ALWAYS(a)
#ifdef PUBLIC_DEBUG
#define IN_FILE_ON_LINE_STRING (__FILE__+":"+__LINE__)
#else
#ifdef _JavaME
#define IN_FILE_ON_LINE_STRING (null)
#elif defined(_android)
#define IN_FILE_ON_LINE_STRING ""
#endif //_JavaME
#endif	//PUBLIC_DEBUG
#define PRINT_STACK_TRACE
#define DEBUG_ASSERT(a)
#else
#ifdef _JavaME
#define DEBUG_ALWAYS(a) MAMidlet.out.print(a); MAMidlet.out.flush()
#elif defined(_android)
#define DEBUG_ALWAYS(a) Log.i("DEBUG MESSAGE",a)
#else
#error Unknown platform!
#endif //_JavaME
#define PRINT_STACK_TRACE \
	DEBUG_ALWAYS("Stack trace " + IN_FILE_ON_LINE_STRING + "\n"); \
	DEBUG_ALWAYS(e.toString() + "\n"); e.printStackTrace()
#define PUBLIC_DEBUG
#define IN_FILE_ON_LINE_STRING ("In "+__FILE__+" on line "+__LINE__)
#define DEBUG_ASSERT(a) MYASSERT(a)
#endif	//PHONE_RELEASE

#if defined(PHONE_RELEASE) && !defined(PUBLIC_DEBUG)
#define BIG_PHAT_ERROR throw new BigPhatError()
#else
#define BIG_PHAT_ERROR throw new BigPhatError(IN_FILE_ON_LINE_STRING)
#endif

#ifdef JDB_DEBUG
#define JDB_EXCEPTION throws Exception
#else
#define JDB_EXCEPTION
#endif

#define MYASSERT(a) if(!(a)) BIG_PHAT_ERROR

#define INVALID_SYSCALL_NUMBER 256
#define SYSCALL_NUMBER_IS_VALID(n) ((n) < INVALID_SYSCALL_NUMBER && (n) >= 0)

#ifdef _DEBUG
#define DEBUG(a) DEBUG_ALWAYS(a)
#define DEBUG_RELEASE(a)
#define DEBUG_MEMORY
#define DEBUG_SYSCALLS
#else
#define DEBUG(a)
#define DEBUG_RELEASE(a) DEBUG_ALWAYS(a)
#endif	//_DEBUG

#ifdef DEBUG_SYSCALLS
#define DEBUG_SC(a) DEBUG_ALWAYS(a)
#else
#define DEBUG_SC(a)
#endif	//DEBUG_SYSCALLS

#if defined(DEBUG_MEMORY) || defined(PUBLIC_DEBUG)
#define UPDATE_IP
#endif

#define LOGSC DEBUG_SC

#define DEBUG_RES(a) DEBUG_ALWAYS(a)
#define DEBUG_BT(a) DEBUG_ALWAYS(a)
#define DEBUG_LOAD(a) DEBUG_ALWAYS(a)
#define DEBUG_SOUND(a) DEBUG_ALWAYS(a)
#define DEBUG_KEYS(a) DEBUG_ALWAYS(a)
#define DEBUG_TEMP(a) DEBUG_ALWAYS(a)	//for stuff that should be removed after bugs are fixed
#define DEBUG_CONN(a) DEBUG_ALWAYS(a)
#define DEBUG_MEDIA_STREAM(a) DEBUG_ALWAYS(a)

#ifdef BFE_EXCEPTION
#define THE	throws Exception
#define CEX , Exception
#else
#define THE
#define CEX
#endif	//BFE_EXCEPTION

#ifdef MOSYNC_STATIC_JAVA
#define YIELD
#define STATIC_SYSCALL(s) _##s
#define STATIC_EXCEPTION_STRING "STATIC_EXIT"
#define STATIC_EXIT throw new Exception(STATIC_EXCEPTION_STRING)
#else
#define YIELD mCore.yield()
#define STATIC_SYSCALL(s) s
#define STATIC_EXIT
#endif

#define FSI final static int

#define IN_FILE_ON_LINE DEBUG_ALWAYS(IN_FILE_ON_LINE_STRING)
#define DUMP(a) DEBUG_ALWAYS(#a+": "+(a)+"\n")
#define DDUMP(a) DEBUG(#a+": 0x"+HEX(a)+"\n")
#define HEX(a) Integer.toHexString(a)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define CAP_int(p) p##Int
#define CAP_short(p) p##Short

#define MAExtent int
#define MAAddress Address
#define Address int
#define MAString String
#define MAHandle int

#define MAPoint2d Address
#define MAPoint2d_x(point) RINT(point)
#define MAPoint2d_y(point) RINT(point + 4)

#define MARect Address
#define MARect_left(rect) RINT(rect)
#define MARect_top(rect) RINT(rect + 4)
#define MARect_width(rect) RINT(rect + 8)
#define MARect_height(rect) RINT(rect + 12)

#define MAEvent Address

#define EXTENT_Y(e) ((short)(e))
#define EXTENT_X(e) ((short)((e) >> 16))
#define EXTENT(x, y) ((MAExtent)((((int)(x)) << 16) | ((y) & 0x0FFFF)))

#define INTPART(addr) ((addr) >>> 2)
#define BYTEPART(addr) ((addr) & 0x3)

//Event Index, see IDL, struct MAEvent
#define EI_TYPE event[0]
#define EI_KEY event[1]
#define EI_NATIVE_KEY event[2]
#define EI_POINTX event[1]
#define EI_POINTY event[2]
#define EI_CONN_HANDLE event[1]
#define EI_CONN_OPTYPE event[2]
#define EI_CONN_RESULT event[3]
#define EI_STATE event[1]
#define EI_DATA event[1]
