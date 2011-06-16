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

#include <MAP/MemoryMgr.h>
#include "AppScreenBase.h"
#include "ShowMenuAction.h"
#include "ExitAction.h"
#include "AppStyleMgr.h"
#include "IKeyHandler.h"

namespace MapDemoUI 
{
	static const int KeyRepeatMs = 150;
	static const int MessagePostTimeMs = 2000;

	//=========================================================================
	class KeyRepeatTimer : public TimerListener
	//=========================================================================
	{
	public:
		KeyRepeatTimer( IKeyHandler* handler, int keyCode ) :
			mHandler( handler ),
			mKeyCode( keyCode ),
			mRepeats( 0 )
		{
		}

		virtual ~KeyRepeatTimer( ) { }

		void runTimerEvent( )
		{
			mRepeats++;
			if ( mRepeats > 2 )
				mHandler->handleKeyPress( mKeyCode );
		}

		IKeyHandler* mHandler;
		int mKeyCode;
		int mRepeats;
	};

	//=========================================================================

	const int BottomHeight = 25;

	//-------------------------------------------------------------------------
	AppScreenBase::AppScreenBase( MobletEx* moblet ) :
	//-------------------------------------------------------------------------
		mMoblet( moblet ),
		mSoftKeyBar( NULL ),
		mContentFrame( NULL ),
		mAppFrame( NULL ),
		mMessagePosted( false ),
		mKeyTimer( NULL )
	{
		//
		// Screen size
		//
		MAExtent screenSize = maGetScrSize( );
		mWidth = EXTENT_X( screenSize );
		mHeight = EXTENT_Y( screenSize );
		//
		// Frame containing tab widgets
		//
		mContentFrame = newobject( Layout, new Layout( 0, 0, mWidth, mHeight - BottomHeight, NULL, 1, 1 ) );
		//
		// Bottom bar with soft key labels
		//
		mSoftKeyBar = newobject( SoftKeyBar, new SoftKeyBar( 0, 0, mWidth, BottomHeight, NULL ) );
		mSoftKeyBar->setLeftAction( newobject( ShowMenuAction, new ShowMenuAction( this ) ) );
		mSoftKeyBar->setRightAction( newobject( ExitAction, new ExitAction( mMoblet ) ) );

		AppStyle* style = AppStyleMgr::getStyle( );
		mAppFrame = newobject( AppFrame, new AppFrame( 0, 0, mWidth, mHeight,NULL, 1, 3 ) );

		mAppFrame->add( mContentFrame );
		mAppFrame->add( mSoftKeyBar );
		mAppFrame->setDrawBackground( false );
		mAppFrame->setfont( style->getFont( FontSize_Smallest, Color::white, false ) );
		mAppFrame->update( );

		setMain( mAppFrame );
		//
		// Listen to messages
		//
		MessageMgr::get( )->addListener( this );
	}

	//-------------------------------------------------------------------------
	AppScreenBase::~AppScreenBase( )
	//-------------------------------------------------------------------------
	{
		MessageMgr::get( )->removeListener( this );

		if ( mKeyTimer != NULL )
		{
			Environment::getEnvironment( ).removeTimer( mKeyTimer );
			deleteobject( mKeyTimer );
		}

		if ( mAppFrame != NULL )
		{
			mAppFrame->getChildren( ).clear( );
			deleteobject( mAppFrame );
		}

		if ( mContentFrame != NULL )
		{
			mContentFrame->getChildren( ).clear( );
			deleteobject( mContentFrame );
		}
		deleteobject( mSoftKeyBar );
	}

	//-------------------------------------------------------------------------
	void AppScreenBase::setClientWidget( Widget* widget )
	//-------------------------------------------------------------------------
	{
		widget->setWidth( mContentFrame->getWidth( ) );
		widget->setHeight( mContentFrame->getHeight( ) );
		if( mContentFrame->getChildren( ).size( ) > 0 )
			mContentFrame->getChildren( ).clear( );
		mContentFrame->add( widget );
	}

	//-------------------------------------------------------------------------
	void AppScreenBase::keyPressEvent( int keyCode, int nativeCode )
	//-------------------------------------------------------------------------
	{
		(void)handleKeyPress( keyCode );
		if ( mKeyTimer == NULL )
		{
			mKeyTimer = newobject( KeyRepeatTimer, new KeyRepeatTimer( this, keyCode ) );
			Environment::getEnvironment( ).addTimer( mKeyTimer, KeyRepeatMs, 0 );
		}
		else
		{
			// n-key rollover
			mKeyTimer->mKeyCode = keyCode;
		}
	}

	//-------------------------------------------------------------------------
	void AppScreenBase::keyReleaseEvent( int keyCode, int nativeCode )
	//-------------------------------------------------------------------------
	{
		if ( mKeyTimer != NULL ) 
		{
			Environment::getEnvironment( ).removeTimer( mKeyTimer );
			deleteobject( mKeyTimer );
		}
		(void)handleKeyRelease( keyCode );
	}

	//-------------------------------------------------------------------------
	bool AppScreenBase::handleKeyPress( int keyCode )
	//-------------------------------------------------------------------------
	{
		if ( this != Screen::currentScreen )
		{
			if ( mKeyTimer != NULL )
			{
				Environment::getEnvironment( ).removeTimer( mKeyTimer );
				deleteobject( mKeyTimer );
			}
			return true;
		}

		if ( mSoftKeyBar->handleKeyPress( keyCode ) )
			return true;

		return false;
	}

	//-------------------------------------------------------------------------
	bool AppScreenBase::handleKeyRelease( int keyCode )
	//-------------------------------------------------------------------------
	{
		if ( mSoftKeyBar->handleKeyRelease( keyCode ) )
			return true;

		return false;
	}

	//-------------------------------------------------------------------------
	void AppScreenBase::pointerPressEvent(MAPoint2d point)
	//-------------------------------------------------------------------------
	{
		(void)handlePointerPress( point );
	}

	//-------------------------------------------------------------------------
	void AppScreenBase::pointerMoveEvent(MAPoint2d point)
	//-------------------------------------------------------------------------
	{
		(void)handlePointerMove( point );
	}

	//-------------------------------------------------------------------------
	void AppScreenBase::pointerReleaseEvent(MAPoint2d point)
	//-------------------------------------------------------------------------
	{
		(void)handlePointerRelease( point );
	}

	//-------------------------------------------------------------------------
	bool AppScreenBase::handlePointerPress( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		if ( mSoftKeyBar->handlePointerPress( point ) )
			return true;

		return false;
	}

	//-------------------------------------------------------------------------
	bool AppScreenBase::handlePointerMove( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		if ( mSoftKeyBar->handlePointerMove( point ) )
			return true;

		return false;
	}

	//-------------------------------------------------------------------------
	bool AppScreenBase::handlePointerRelease( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		if ( mSoftKeyBar->handlePointerRelease( point ) )
			return true;

		return false;
	}

	//-------------------------------------------------------------------------
	void AppScreenBase::messagePosted( MessageMgr* sender )
	//-------------------------------------------------------------------------
	{
		mAppFrame->setMessage( sender->getMessage( ), sender->getProgress( ) );

		// start timer
		Environment::getEnvironment( ).addTimer( this, MessagePostTimeMs, 1 );
	}

	//-------------------------------------------------------------------------
	void AppScreenBase::runTimerEvent( )
	//-------------------------------------------------------------------------
	{
		mAppFrame->clearMessage( );
	}
};
