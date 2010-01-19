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
using namespace MAPUtil;

namespace MapDemoUtil
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

		static MessageMgr*		get( ) { if ( sSingleton == NULL ) { sSingleton = newobject( MessageMgr, new MessageMgr( ) ); } return sSingleton; }
		static void				shutdown( ) { deleteobject( sSingleton ); }
		void					postMessage( const char *fmt, ... );
		void					postProgress( float progress );
		const char*				getMessage( ) const { return mMessage.c_str( ); }
		float					getProgress( ) const { return mProgress; }

	private:
		void					onMessagePosted( );

		static MessageMgr*		sSingleton;
		String					mMessage;
		float					mProgress;
	};
}

#endif // MESSAGEMGR_H_
