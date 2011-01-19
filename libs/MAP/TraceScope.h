//
// TraceScope.h
//
// Author: Lars Ake Vinberg
//

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
	//=========================================================================
	//
	// Prints a debug trace on creation, and one on destruction.
	// Usage:
	//
	//	...
	//	{
	//		TraceScope scope( "MyFunc" );
	//		...
	//	}
	//	...
	//
	// Output:
	//
	//	+++ MyFunc
	//	--- 45 MyFunc
	class TraceScope
	//=========================================================================
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
