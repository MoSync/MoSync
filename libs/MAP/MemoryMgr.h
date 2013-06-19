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
* \file MemoryMgr.h
* \brief Utility for heap resource tracking and debugging
* \author Lars-Åke Vinberg
*/

#ifndef MEMORYMGR_H_
#define MEMORYMGR_H_

//
// Define to track memory leaks
//
#ifdef _MSC_VER
#define TRACKOBJECTS
#endif

#ifndef __WINDOWS_PHONE_8_LIB__
#include <ma.h>
#include <maheap.h>
#else
#include "../MAStd/ma.h"
#include "../MAStd/maheap.h"
#endif

#ifdef TRACKOBJECTS
#ifndef __WINDOWS_PHONE_8_LIB__
#include <MAUtil/Map.h>
#include <MAUtil/Vector.h>
#else
#include "../MAUtil/Map.h"
#include "../MAUtil/Vector.h"
#endif //__WINDOWS_PHONE_8__

#include "DebugPrintf.h"
using namespace MAUtil;

#endif //TRACKOBJECTS

namespace MAPUtil
{

#ifdef TRACKOBJECTS
	using namespace MAUtil;
#endif
	//=========================================================================
	/**
	 * \brief Simple resource tracking class.
	 */
	class MemoryMgr
	//=========================================================================
	{
	public:
		/**
		 * Registers an object for resource tracking.
		 */
		template<class T>
		static T* track( T* p, const char* label )
		{
			#ifdef TRACKOBJECTS
			mKeys.add( p );
			mValues.add( label );
			#endif
			return p;
		}
		/**
		 * Unregisters an object for resource tracking.
		 */
		static void untrack( void* p )
		{
			#ifdef TRACKOBJECTS
			for ( int i = 0; i < mKeys.size( ); i++ )
			{
				if ( mKeys[i] == p )
				{
					mKeys.remove( i );
					mValues.remove( i );
					return;
				}
			}
			#endif
		}
		/**
		 * Prints a list of currently tracked objects.
		 */
		static void dump( )
		{
			#ifdef TRACKOBJECTS

			int count = mKeys.size( );
			DebugPrintf( "=== Dump: %d objects remaining\n", count );
			for ( int i = 0; i < count; i++ )
				DebugPrintf( "   %s\n", mValues[ i ] );
			DebugPrintf( "=== End dump: %d objects remaining\n", count );

			#endif

		}

	private:
		#ifdef TRACKOBJECTS
		static Vector<void*> mKeys;
		static Vector<const char*> mValues;
		#endif
	};

#ifdef TRACKOBJECTS
	#define newobject( type, obj ) MemoryMgr::track<type>( (obj), #type "   (" __FUNCTION__ ")" )
	#define deleteobject( obj ) \
	{\
		if ( (obj) != NULL )\
		{\
			MemoryMgr::untrack( obj );\
			delete (obj);\
			(obj) = NULL;\
		}\
	}
	#define deleteconstobject( obj ) \
	{\
		if ( (obj) != NULL )\
		{\
			MemoryMgr::untrack( obj );\
			delete (obj);\
		}\
	}
#else
	#define newobject( type, obj ) ( obj )
	#define deleteobject( obj )\
	{\
		if ( (obj) != NULL )\
		{\
			delete (obj);\
			(obj) = NULL;\
		}\
	}
	#define deleteconstobject( obj )\
	{\
		if ( (obj) != NULL )\
		{\
			delete (obj);\
		}\
	}
#endif

}

#endif // MEMORYMGR_H_
