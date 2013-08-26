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
