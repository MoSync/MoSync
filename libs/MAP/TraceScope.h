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
 * \file TraceScope.h
 * \brief Defines a class for tracing and timing blocks.
 * \author Lars Ake Vinberg
 */

#ifndef TRACESCOPE_H_
#define TRACESCOPE_H_

//
// Set to true to enable tracing
//
#define ENABLETRACESCOPE


#include "conprint.h"
#include "DebugPrintf.h"

namespace MAP
{
	/**
	 * @brief Utility class for timing and marking the start and end of a block.
	 *
	 * Prints a debug trace on creation, and one on destruction.
	 * Usage:
	 *
	 *	...
	 *	{
	 *		TraceScope scope( "MyFunc" );
	 *		...
	 *	}
	 *	...
	 *
	 * Output:
	 *
	 *	+++ MyFunc
	 *	--- 45 MyFunc
	 */
	class TraceScope
	{
	public:
		TraceScope( const char* s )			
		{
			m_s = s; 
			DebugPrintf( "+++ %s\n", s ); 
			m_startTime = maGetMilliSecondCount( );
		}

		virtual ~TraceScope( )					
		{
			int time = maGetMilliSecondCount( ) - m_startTime;
			DebugPrintf( "--- %d %s\n", time, m_s ); 
		}

	private:
		const char*		m_s;
		int			m_startTime;
	};

	#ifdef ENABLETRACESCOPE
		#define trace( ) TraceScope tr = TraceScope( __FUNCTION__ )
#define traceline( ) DebugPrintf( "Line %d\n", __LINE__ )
	#else
		#define trace( ) {}
		#define traceline( ) { }
	#endif

}
#endif // TRACESCOPE_H_
