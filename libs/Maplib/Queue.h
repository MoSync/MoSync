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

namespace Util
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
			: m_capacity( capacity ),
			m_items( NULL )
		{
			m_items = newobject( T*, new T*[capacity] );
			for ( int i = 0; i < m_capacity; i++ )
				m_items[i] = NULL;
			m_enqueuePosition = 0;
			m_dequeuePosition = 0;
		}
		//
		// Destruct queue
		//
		virtual ~Queue( )
		{
			// Delete each item
			clear( );
			// Delete array
			deleteobject( m_items );
		}
		//
		// Returns item at location in queue.
		// Returns InvalidPtr if item is unused.
		//
		T* peekAt( int location )
		{
			return m_items[location];
		}
		//
		// Returns item capacity of queue.
		//
		int getCapacity( ) const { return m_capacity; }
		//
		// Returns count of queued items.
		//
		int getCount( ) const
		{
			int count = m_enqueuePosition - m_dequeuePosition;
			if ( count < 0 )
				count += m_capacity;
			return count;
		}
		//
		// Removes all items in queue.
		//
		void clear( )
		{
			for ( int i = 0; i < m_capacity; i++ )
				deleteobject( m_items[i] );
			m_enqueuePosition = 0;
			m_dequeuePosition = 0;
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
				if ( m_items[i] != NULL )
					if ( m_items[i] == item )
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
			T* ret = m_items[m_dequeuePosition];
			m_items[m_dequeuePosition] = NULL;
			m_dequeuePosition++;
			if ( m_dequeuePosition >= m_capacity )
				m_dequeuePosition = 0;
			return ret;
		}
		//
		// Enters T into queue.
		// Queue takes ownership of item.
		//
		void enqueue( T* item )
		{
			// TODO: raise exception
			if ( getCount( ) >= m_capacity)
				return;
			m_items[m_enqueuePosition] = item;
			m_enqueuePosition ++;
			if ( m_enqueuePosition >= m_capacity )
				m_enqueuePosition = 0;
		}
		//
		// Returns next item in queue, without removing it from queue.
		//
		const T* peek( ) const
		{
			if ( getCount( ) == 0 )
				return NULL;
			return m_items[m_dequeuePosition];
		}

	private:
		T**						m_items;
		int						m_capacity;
		int						m_enqueuePosition;
		int						m_dequeuePosition;
	};
}

#endif // QUEUE_H_
