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

#ifndef MODALSCREEN_H_
#define MODALSCREEN_H_

#include <MAUI/Screen.h>
#include "IKeyHandler.h"
#include <MAP/Broadcaster.h>
#include "AppScreenBase.h"
#include "IClosable.h"

using namespace MAUI;
using namespace MapDemoUtil;

namespace MapDemoUI 
{
	//
	// Forward declarations
	class ModalScreen;

	//=========================================================================
	//
	// Listener to ModalScreen class
	//
	class IModalScreenListener
	//=========================================================================
	{
	public:
		virtual void screenClosed( ModalScreen* sender ) = 0;
	};

	//=========================================================================
	//
	// Abstract class.
	//
	class ModalScreen : public Screen, 
		public IKeyHandler, 
		public Broadcaster<IModalScreenListener>,
		public IClosable
	//=========================================================================
	{
	public:
		ModalScreen( );
		
		virtual ~ModalScreen( );
		//
		// public methods
		//
		void close( );
		//
		// Screen overrides
		//
		void keyPressEvent( int keyCode, int nativeCode ); // client can't override

		void keyReleaseEvent( int keyCode, int nativeCode ); // client can't override

		void pointerPressEvent(MAPoint2d point);

		void pointerMoveEvent(MAPoint2d point);

		void pointerReleaseEvent(MAPoint2d point);

		virtual void show( );
		
		virtual void show( Screen* previous );
		//
		// IKeyHandler implementation
		//
		virtual bool handleKeyPress( int keyCode ) = 0;
		virtual bool handleKeyRelease( int keyCode ) = 0;
		//
		// IPointerHandler implementation
		//
		virtual bool handlePointerPress( MAPoint2d point ) = 0;
		virtual bool handlePointerMove( MAPoint2d point ) = 0;
		virtual bool handlePointerRelease( MAPoint2d point ) = 0;

	protected:
		int mWidth;
		int mHeight;

	private:
		Screen* mPrevious;
	};
}

#endif // MODALSCREEN_H_
