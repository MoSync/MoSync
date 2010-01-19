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

#ifndef QUEUE_H_
#define QUEUE_H_

#include "MemoryMgr.h"

namespace MAPUtil
{
	//=========================================================================
	// Template class for a queue with a limited fixed capacity.
	//
	template<class T>
	class Queue
	//=========================================================================
	{
	public:
		//
		// Construct a queue with fixed capacity.
		// Queue keeps heap-allocated objects of type T.
		// Client must create objects.
		// Queue assumes ownership of objects as long as objects are in queue.
		// Dequeueing an object means ownership is transferred to client.
		//
		Queue( const int capacity )
			: mCapacity( capacity ),
			mItems( NULL )
		{
			mItems = newobject( T*, new T*[capacity] );
			for ( int i = 0; i < mCapacity; i++ )
				mItems[i] = NULL;
			mEnqueuePosition = 0;
			mDequeuePosition = 0;
		}
		//
		// Destruct queue
		//
		virtual ~Queue( )
		{
			// Delete each item
			clear( );
			// Delete array
			deleteobject( mItems );
		}
		//
		// Returns item at location in queue.
		// Returns InvalidPtr if item is unused.
		//
		T* peekAt( int location )
		{
			return mItems[location];
		}
		//
		// Returns item capacity of queue.
		//
		int getCapacity( ) const { return mCapacity; }
		//
		// Returns count of queued items.
		//
		int getCount( ) const
		{
			int count = mEnqueuePosition - mDequeuePosition;
			if ( count < 0 )
				count += mCapacity;
			return count;
		}
		//
		// Removes all items in queue.
		//
		void clear( )
		{
			for ( int i = 0; i < mCapacity; i++ )
				deleteobject( mItems[i] );
			mEnqueuePosition = 0;
			mDequeuePosition = 0;
		}
		//
		// returns true if queue contains item.
		//
		bool contains( const T* item ) const { return Find( item ) != -1; }
		//
		// Returns position in array of item.
		// If not found, returns -1.
		//
		int find ( const T* item ) const
		{
			int count = getCount( );
			for ( int i = 0; i < count; i++ )
				if ( mItems[i] != NULL )
					if ( mItems[i] == item )
						return i;
			return -1;
		}
		//
		// Returns ref to next T in queue.
		// Caller assumes ownership of item, and is responsible for deleting item.
		// Returns InvalidPtr if queue is empty.
		//
		T* dequeue( )
		{
			if ( getCount( ) == 0 )
				return NULL;
			T* ret = mItems[mDequeuePosition];
			mItems[mDequeuePosition] = NULL;
			mDequeuePosition++;
			if ( mDequeuePosition >= mCapacity )
				mDequeuePosition = 0;
			return ret;
		}
		//
		// Enters T into queue.
		// Queue takes ownership of item.
		//
		void enqueue( T* item )
		{
			// TODO: raise exception
			if ( getCount( ) >= mCapacity)
				return;
			mItems[mEnqueuePosition] = item;
			mEnqueuePosition ++;
			if ( mEnqueuePosition >= mCapacity )
				mEnqueuePosition = 0;
		}
		//
		// Returns next item in queue, without removing it from queue.
		//
		const T* peek( ) const
		{
			if ( getCount( ) == 0 )
				return NULL;
			return mItems[mDequeuePosition];
		}

	private:
		T**						mItems;
		int						mCapacity;
		int						mEnqueuePosition;
		int						mDequeuePosition;
	};
}

#endif // QUEUE_H_
