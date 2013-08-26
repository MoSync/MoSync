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

#ifndef MESSAGEMGR_H_
#define MESSAGEMGR_H_

#include <MAUtil/Vector.h>
#include <MAUtil/String.h>

#include <MAP/MemoryMgr.h>
#include <MAP/Broadcaster.h>

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
	private:
		MessageMgr( );

	public:
		virtual ~MessageMgr( );

		static MessageMgr* get( )
		{
			if ( sSingleton == NULL )
			{
				sSingleton = newobject( MessageMgr, new MessageMgr( ) );
			}
			return sSingleton;
		}

		static void shutdown( )
		{
			deleteobject( sSingleton );
		}

		void postMessage( const char *fmt, ... ) GCCATTRIB(format(printf, 2, 3));

		void postProgress( float progress );

		const char* getMessage( ) const
		{
			return mMessage.c_str( );
		}

		float getProgress( ) const
		{
			return mProgress;
		}

	private:
		void onMessagePosted( );

		static MessageMgr* sSingleton;
		String mMessage;
		float mProgress;
	};
}

#endif // MESSAGEMGR_H_
