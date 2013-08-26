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
