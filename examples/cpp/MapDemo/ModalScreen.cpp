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

#include "ModalScreen.h"
#include <MAP/Broadcaster.h>

using namespace MapDemoUtil;

namespace MapDemoUI 
{
	//-------------------------------------------------------------------------
	ModalScreen::ModalScreen( )
	//-------------------------------------------------------------------------
	: Screen( )
	{
		//
		// Screen size
		//
		MAExtent screenSize = maGetScrSize( );
		mWidth = EXTENT_X( screenSize );
		mHeight = EXTENT_Y( screenSize );
	}

	//-------------------------------------------------------------------------
	ModalScreen::~ModalScreen( )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	void ModalScreen::show( )
	//-------------------------------------------------------------------------
	{
		show( Screen::currentScreen );
	}

	//-------------------------------------------------------------------------
	void ModalScreen::show( Screen* previous )
	//-------------------------------------------------------------------------
	{
		mPrevious = previous;
		Screen::show( );
	}

	//-------------------------------------------------------------------------
	//
	// Deletes this, so this pointer is invalid on return.
	// Must be last reference to object.
	//
	void ModalScreen::close( )
	//-------------------------------------------------------------------------
	{
		mPrevious->show( );
		Vector<IModalScreenListener*>* listeners = getBroadcasterListeners<IModalScreenListener>( *this );
		for ( int i = 0; i < listeners->size(); i++ )
			(*listeners)[i]->screenClosed( this );
		
		deleteconstobject( this );
	}

	//-------------------------------------------------------------------------
	void ModalScreen::keyPressEvent( int keyCode, int nativeCode )
	//-------------------------------------------------------------------------
	{
		(void)handleKeyPress( keyCode );
	}

	//-------------------------------------------------------------------------
	void ModalScreen::keyReleaseEvent( int keyCode, int nativeCode )
	//-------------------------------------------------------------------------
	{
		(void)handleKeyRelease( keyCode );
	}

	//-------------------------------------------------------------------------
	void ModalScreen::pointerPressEvent(MAPoint2d point)
	//-------------------------------------------------------------------------
	{
		(void)handlePointerPress( point );
	}

	//-------------------------------------------------------------------------
	void ModalScreen::pointerMoveEvent(MAPoint2d point)
	//-------------------------------------------------------------------------
	{
		(void)handlePointerMove( point );
	}

	//-------------------------------------------------------------------------
	void ModalScreen::pointerReleaseEvent(MAPoint2d point)
	//-------------------------------------------------------------------------
	{
		(void)handlePointerRelease( point );
	}

	//-------------------------------------------------------------------------
	bool ModalScreen::handlePointerPress( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		return false;
	}

	//-------------------------------------------------------------------------
	bool ModalScreen::handlePointerMove( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		return false;
	}

	//-------------------------------------------------------------------------
	bool ModalScreen::handlePointerRelease( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		return false;
	}
}
