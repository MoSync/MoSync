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

#ifndef BROADCASTER_H_
#define BROADCASTER_H_

#include <MAUtil/Vector.h>

using namespace MAUtil;

namespace Util
{
	//=========================================================================
	template<class T>
	class Broadcaster
	//=========================================================================
	{
	public:
							Broadcaster( ) { }
		virtual				~Broadcaster( ) { }

		//
		// listener property
		//
		void				addListener( T* listener ) { m_listeners.add( listener ); }
		void				removeListener( T* listener ) 
							{ 
								for ( int i = 0; i < m_listeners.size( ); i++ ) 
								{
									if ( m_listeners[i] == listener )
									{
										m_listeners.remove( i );
										return;
									}
								}
							}
	protected:
		Vector<T*>			m_listeners;
	};
}

#endif // BROADCASTER_H_
