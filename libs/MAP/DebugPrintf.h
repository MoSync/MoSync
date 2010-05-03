/* Copyright (C) 2010 Mobile Sorcery AB

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

/** 
* \file DebugPrintf.h
* \brief Utility functions for debug output under MSVC compiler
* \author Lars-Åke Vinberg
*/

#ifndef DEBUGPRINTF_H_
#define DEBUGPRINTF_H_

#ifdef _MSC_VER

// To speed up compiles, the declarations we need are copied below.
// #include "windows.h"
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA( /*__in_opt*/ const char* lpOutputString );
#define OutputDebugString  OutputDebugStringA

#include <stdarg.h>

#else
#include <conprint.h>
#endif
#include <mavsprintf.h>

//
// Printf statement for trace prints to debugger output,
// for example in the Visual Studio environment.
//
inline void DebugPrintf(const char *fmt, ...)
{
	#ifdef _MSC_VER
	
	//
	// Windows implementation
	//
	char buffer[32768];
	va_list vl;
	va_start( vl, fmt );
	vsprintf( buffer, fmt, vl );
	va_end( vl );
	OutputDebugString( buffer );

	#else
	
	//
	// do nothing
	//

	#endif
}

//
// Assert statement for x86 debuggers.
// Will trigger a debugger breakpoint if a debugger attached.
// If no debugger is attached then the function does nothing.
//
inline void DebugAssert( bool condition )
{
	#ifdef _MSC_VER

	//
	// Windows implementation
	//
	if ( !condition )
	{
		__asm { int 3 }
	}

	#else

	//
	// do nothing
	//

	#endif
}

#endif // DEBUGPRINTF_H_
