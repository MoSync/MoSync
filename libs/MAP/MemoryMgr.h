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
