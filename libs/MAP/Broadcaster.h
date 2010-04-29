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
* \file Broadcaster.h
* \brief Typed notification broadcasting to multiple listeners
* \author Lars-Åke Vinberg
*/

#ifndef BROADCASTER_H_
#define BROADCASTER_H_

#include <MAUtil/Vector.h>

using namespace MAUtil;

namespace MAPUtil
{
	//=========================================================================
	/**
	 * This template class enables typed notification broadcasting to multiple
	 * listeners.
	 * Listeners must implement class T.
	 * 
	 * A listener adds itself to the list of listeners by calling AddListener( ) on
	 * the object it wants to listen to.
	 * 
	 * To broadcast, loop through all listeners in mListeners and call the appropriate
	 * notification function.
	 *
	 * Example:
	 *
	 *    ...
	 *    for ( int i = 0; i < mListeners.size( ); i++ )
	 *       m_Listeners[i]->notify( );
	 * 
	 */
	template<class T>
	class Broadcaster
	//=========================================================================
	{
	public:
		/**
		* Add a listener.
		* Clients should call this to register for notification.
		*/
		void addListener( T* listener ) 
		{
			mListeners.add( listener );
		}
		/**
		 * Remove a listener.
		 * Clients should call this to unregister for notification.
		 */
		void removeListener( T* listener ) 
		{ 
			for ( int i = 0; i < mListeners.size( ); i++ ) 
			{
				if ( mListeners[i] == listener )
				{
					mListeners.remove( i );
					return;
				}
			}
		}

	protected:
		/**
		 * The list of listeners.
		 * Implementer should iterate through list and call the appropriate
		 * notify function.
		 */
		Vector<T*> mListeners;
	};
}

#endif // BROADCASTER_H_
