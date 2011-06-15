/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

#include <mavsprintf.h>

#include "MessageMgr.h"
#include <MAP/Broadcaster.h>

using namespace MAPUtil;

namespace MapDemoUtil
{
	MessageMgr* MessageMgr::sSingleton = NULL;

	//-------------------------------------------------------------------------
	MessageMgr::MessageMgr( ) :
	//-------------------------------------------------------------------------
		mMessage( ),
		mProgress( 0 )
	{
	}

	//-------------------------------------------------------------------------
	MessageMgr::~MessageMgr( )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	void MessageMgr::postMessage( const char *fmt, ... )
	//-------------------------------------------------------------------------
	{
		char buffer[1024];
		va_list vl;
		va_start( vl, fmt );
		vsprintf( buffer, fmt, vl );
		va_end( vl );

		mMessage = buffer;
		mProgress = -1;
		onMessagePosted( );
	}

	//-------------------------------------------------------------------------
	void MessageMgr::postProgress( float progress )
	//-------------------------------------------------------------------------
	{
		mProgress = progress;
		onMessagePosted( );
	}

	//-------------------------------------------------------------------------
	void MessageMgr::onMessagePosted( )
	//-------------------------------------------------------------------------
	{
		Vector<IMessageListener*>* listeners = getBroadcasterListeners<IMessageListener>( *this );
		int count = listeners->size( );
		for ( int i = 0; i < count; i++ )
			(*listeners)[i]->messagePosted( this );
	}
}
