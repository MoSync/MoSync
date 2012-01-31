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
