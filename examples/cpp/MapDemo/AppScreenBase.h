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

#ifndef APPSCREENBASE_H_
#define APPSCREENBASE_H_

#include <MAUI/Screen.h>

#include "SoftKeyBar.h"
#include "MobletEx.h"
#include "MessageMgr.h"
#include "AppFrame.h"
#include "IActionSource.h"
#include "IKeyHandler.h"
#include "IPointerHandler.h"

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
		IPointerHandler,
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
		void pointerPressEvent(MAPoint2d point);
	    void pointerMoveEvent(MAPoint2d point);
	    void pointerReleaseEvent(MAPoint2d point);
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
		// IPointerHandler implementation
		//
		virtual bool handlePointerPress( MAPoint2d point );
		virtual bool handlePointerMove( MAPoint2d point );
		virtual bool handlePointerRelease( MAPoint2d point );
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
