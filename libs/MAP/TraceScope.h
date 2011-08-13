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
