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

#ifndef APPSCREENBASE_H_
#define APPSCREENBASE_H_

#include <MAUI/Screen.h>

#include "SoftKeyBar.h"
#include "MobletEx.h"
#include "MessageMgr.h"
#include "AppFrame.h"
#include "IActionSource.h"
#include "IKeyHandler.h"

using namespace MAUI;
using namespace MAUtil;

namespace MapDemoUI 
{
	class KeyRepeatTimer;

	//=========================================================================
	//
	// Abstract base class for AppScreen variations.
	// Handles menus, messages, modal screens
	//
	class AppScreenBase : public Screen, 
		IMessageListener, 
		TimerListener, 
		IKeyHandler, 
		public IActionSource
	//=========================================================================
	{
	public:
		AppScreenBase( MobletEx* mMoblet );
		
		virtual ~AppScreenBase( );

		void setClientWidget( Widget* widget );
		
		static Screen* getCurrentScreen( ) 
		{
			return currentScreen; 
		}	
		
		Moblet* getMoblet( ) 
		{ 
			return mMoblet; 
		}

		//
		// Screen overrides
		//
		void keyPressEvent( int keyCode, int nativeCode );
		void keyReleaseEvent( int keyCode, int nativeCode );
		//
		// IMessageListener implementation
		//
		virtual void messagePosted( MessageMgr* sender );
		//
		// TimerListener implementation
		//
		virtual void runTimerEvent( );
		//
		// IKeyHandler implementation
		//
		virtual bool handleKeyPress( int keyCode );
		virtual bool handleKeyRelease( int keyCode );
		//
		// IActionSource implementation
		//
		virtual void enumerateActions( Vector<Action*>& list ) = 0;

	protected:
		MobletEx* mMoblet;
		int mWidth;
		int mHeight;
		SoftKeyBar* mSoftKeyBar;

	private:
		Layout* mContentFrame;
		AppFrame* mAppFrame;
		bool mMessagePosted;
		KeyRepeatTimer* mKeyTimer;
	};
};

#endif // APPSCREENBASE_H_
