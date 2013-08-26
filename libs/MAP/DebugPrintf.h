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
#ifndef __WINDOWS_PHONE_8_LIB__
#include <mavsprintf.h>
#else
#include "../MAStd/mavsprintf.h"
#endif

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
#ifndef __WINDOWS_PHONE_8_MAP_LIB__
		__asm { int 3 }
#endif
	}

	#else

	//
	// do nothing
	//

	#endif
}

#endif // DEBUGPRINTF_H_
