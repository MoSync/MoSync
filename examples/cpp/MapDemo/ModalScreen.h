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
