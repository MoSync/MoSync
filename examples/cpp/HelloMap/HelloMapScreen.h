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

#ifndef HELLOMAPSCREEN_H_
#define HELLOMAPSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUtil/Moblet.h>
#include <MAP/MapWidget.h>
#include <MAP/MapSource.h>

using namespace MAP;

namespace HelloMap
{
	//=========================================================================
	//
	// Screen for HelloMap app
	//
	class HelloMapScreen : public Screen
	//=========================================================================
	{
	public:
		HelloMapScreen( );
		virtual	~HelloMapScreen( );
		//
		// Screen overrides
		//
		void keyPressEvent( int keyCode, int nativeCode );
		void keyReleaseEvent( int keyCode, int nativeCode );
		//
		// PointerListener overrides
		//
		void pointerPressEvent(MAPoint2d p);
		void pointerMoveEvent(MAPoint2d p);
		void pointerReleaseEvent(MAPoint2d p);
		// TODO: Shouldn't be needed
		void multitouchPressEvent(MAPoint2d point, int touchId) {}
		void multitouchMoveEvent(MAPoint2d point, int touchId) {}
		void multitouchReleaseEvent(MAPoint2d point, int touchId) {}

	private:
		void					nextMapSource( );
		void					setPosition( MAPoint2d p );

		MapWidget*				mMap;
		Font*					mFont;
		MapSource*				mOpenStreetMapSource;
		MapSource*				mGoogleStreetMapSource;
		MapSource*				mGoogleAerialMapSource;
		MapSource*				mGoogleHybridMapSource;
	};
}

#endif // HELLOMAPSCREEN_H_
