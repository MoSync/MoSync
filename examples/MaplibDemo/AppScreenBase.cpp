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

#include "MemoryMgr.h"
#include "AppScreenBase.h"
#include "ShowMenuAction.h"
#include "ExitAction.h"
#include "AppStyleMgr.h"
#include "IKeyHandler.h"
//#include "TraceScope.h"

namespace UI 
{
	static const bool Trace = false;

	static const int KeyRepeatMs = 150;
	static const int MessagePostTimeMs = 2000;

	//=========================================================================
	class KeyRepeatTimer : public TimerListener
	//=========================================================================
	{
	public:
		KeyRepeatTimer( IKeyHandler* handler, int keyCode ) :
			m_handler( handler ),
			m_keyCode( keyCode ),
			m_repeats( 0 )
		{
		}

		virtual ~KeyRepeatTimer( ) { }

		void runTimerEvent( )
		{
			m_repeats++;
			if ( m_repeats > 2 )
				m_handler->handleKeyPress( m_keyCode );
		}

	//private:
		IKeyHandler*		m_handler;
		int					m_keyCode;
		int					m_repeats;
	};

	//=========================================================================

	const int BottomHeight = 25;

	//-------------------------------------------------------------------------
	AppScreenBase::AppScreenBase( MobletEx* moblet ) :
	//-------------------------------------------------------------------------
		m_moblet( moblet ),
		m_messagePosted( false ),
		m_keyTimer( NULL ),
		m_contentFrame( NULL ),
		m_softKeyBar( NULL ),
		m_appFrame( NULL )
	{
		//if ( Trace ) trace( );

		//
		// Screen size
		//
		MAExtent screenSize = maGetScrSize( );
		m_width = EXTENT_X( screenSize );
		m_height = EXTENT_Y( screenSize );
		//
		// Frame containing tab widgets
		//
		m_contentFrame = newobject( Layout, new Layout( 0, 0, m_width, m_height - BottomHeight, NULL, 1, 1 ) );
		//
		// Bottom bar with soft key labels
		//
		m_softKeyBar = newobject( SoftKeyBar, new SoftKeyBar( 0, 0, m_width, BottomHeight, NULL ) );
		m_softKeyBar->setLeftAction( newobject( ShowMenuAction, new ShowMenuAction( this ) ) );
		m_softKeyBar->setRightAction( newobject( ExitAction, new ExitAction( m_moblet ) ) );

		AppStyle* style = AppStyleMgr::getStyle( );
		m_appFrame = newobject( AppFrame, new AppFrame( 0, 0, m_width, m_height,NULL, 1, 3 ) );

		m_appFrame->add( m_contentFrame );
		m_appFrame->add( m_softKeyBar );
		m_appFrame->setDrawBackground( false );
		m_appFrame->setfont( style->getFont( FontSize_Smallest, Color::white, false ) );
		m_appFrame->update( );

		setMain( m_appFrame );
		//
		// Listen to messages
		//
		MessageMgr::get( )->addListener( this );
	}

	//-------------------------------------------------------------------------
	AppScreenBase::~AppScreenBase( )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		MessageMgr::get( )->removeListener( this );

		if ( m_appFrame != NULL )
		{
			m_appFrame->getChildren( ).clear( );
			deleteobject( m_appFrame );
		}

		if ( m_contentFrame != NULL )
		{
			m_contentFrame->getChildren( ).clear( );
			deleteobject( m_contentFrame );
		}
		deleteobject( m_softKeyBar );
	}

	//-------------------------------------------------------------------------
	void AppScreenBase::setClientWidget( Widget* widget )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		widget->setWidth( m_contentFrame->getWidth( ) );
		widget->setHeight( m_contentFrame->getHeight( ) );
		if( m_contentFrame->getChildren( ).size( ) > 0 )
			m_contentFrame->getChildren( ).clear( );
		m_contentFrame->add( widget );
	}

	//-------------------------------------------------------------------------
	void AppScreenBase::keyPressEvent( int keyCode )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		(void)handleKeyPress( keyCode );
		if ( m_keyTimer == NULL )
		{
			m_keyTimer = newobject( KeyRepeatTimer, new KeyRepeatTimer( this, keyCode ) );
			Environment::getEnvironment( ).addTimer( m_keyTimer, KeyRepeatMs, 0 );
		}
		else
		{
			// n-key rollover
			m_keyTimer->m_keyCode = keyCode;
		}
	}

	//-------------------------------------------------------------------------
	void AppScreenBase::keyReleaseEvent( int keyCode )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		if ( m_keyTimer != NULL ) 
		{
			Environment::getEnvironment( ).removeTimer( m_keyTimer );
			deleteobject( m_keyTimer );
		}
		(void)handleKeyRelease( keyCode );
	}

	//-------------------------------------------------------------------------
	bool AppScreenBase::handleKeyPress( int keyCode )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		if ( this != Screen::currentScreen )
		{
			if ( m_keyTimer != NULL )
			{
				Environment::getEnvironment( ).removeTimer( m_keyTimer );
				deleteobject( m_keyTimer );
			}
			return true;
		}

		if ( m_softKeyBar->handleKeyPress( keyCode ) )
			return true;

		return false;
	}

	//-------------------------------------------------------------------------
	bool AppScreenBase::handleKeyRelease( int keyCode )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		if ( m_softKeyBar->handleKeyRelease( keyCode ) )
			return true;

		return false;
	}

	//-------------------------------------------------------------------------
	void AppScreenBase::messagePosted( MessageMgr* sender )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		m_appFrame->setMessage( sender->getMessage( ), sender->getProgress( ) );

		// start timer
		Environment::getEnvironment( ).addTimer( this, MessagePostTimeMs, 1 );
	}

	//-------------------------------------------------------------------------
	void AppScreenBase::runTimerEvent( )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		m_appFrame->clearMessage( );
	}
};
