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

#ifndef MESSAGEMGR_H_
#define MESSAGEMGR_H_

#include <MAUtil/Vector.h>
#include <MAUtil/String.h>

#include "MemoryMgr.h"
#include "Broadcaster.h"

using namespace MAUtil;

namespace Util
{
	class MessageMgr;

	//============================================================================
	class IMessageListener
	//============================================================================
	{
	public:
		virtual void messagePosted( MessageMgr* sender ) = 0;
	};

	//============================================================================
	// Implemented as singleton.
	//
	class MessageMgr : public Broadcaster<IMessageListener>
	//============================================================================
	{
	public:
	private:
								MessageMgr( );
	public:
		virtual					~MessageMgr( );

		static MessageMgr*		get( ) { if ( s_singleton == NULL ) { s_singleton = newobject( MessageMgr, new MessageMgr( ) ); } return s_singleton; }
		static void				shutdown( ) { deleteobject( s_singleton ); }
		void					postMessage( const char *fmt, ... );
		void					postProgress( float progress );
		const char*				getMessage( ) const { return m_message.c_str( ); }
		float					getProgress( ) const { return m_progress; }

	private:
		void					onMessagePosted( );

		static MessageMgr*		s_singleton;
		String					m_message;
		float					m_progress;
	};
}

#endif // MESSAGEMGR_H_
