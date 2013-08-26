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

#ifndef MENU_H_
#define MENU_H_

#include <MAUI/ListBox.h>

#include "ModalScreen.h"

using namespace MapDemoUtil;

namespace MapDemoUI
{
	class IActionSource;
	class SoftKeyBar;

	//=========================================================================
	//
	// Base class for menus
	//
	class Menu : public ModalScreen
	//=========================================================================
	{
	public:
		Menu( IActionSource* source );
		virtual ~Menu( );
		//
		// public methods
		//
		void triggerSelectedItem( );

		//
		// ModalScreen overrides
		//
		virtual bool handleKeyPress( int keyCode );
		virtual bool handleKeyRelease( int keyCode );
		virtual bool handlePointerPress( MAPoint2d point );
		virtual bool handlePointerMove( MAPoint2d point );
		virtual bool handlePointerRelease( MAPoint2d point );
		virtual void multitouchPressEvent(MAPoint2d p, int touchId) { }
		virtual void multitouchMoveEvent(MAPoint2d p, int touchId) { }
		virtual void multitouchReleaseEvent(MAPoint2d p, int touchId) { }

	protected:

	private:
		void updateSelection( );

		Layout* mFrame;
		ListBox* mListBox;
		SoftKeyBar* mSoftKeys;
		int mCurrent;
	};
}

#endif // MENU_H_
