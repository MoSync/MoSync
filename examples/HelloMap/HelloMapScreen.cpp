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

#include "HelloMapScreen.h"
#include "MAHeaders.h"
#include <mastdlib.h>

namespace HelloMap
{
	//-------------------------------------------------------------------------
	HelloMapScreen::HelloMapScreen( )
	//-------------------------------------------------------------------------
	{
		//
		// Screen size
		//
		MAExtent screenSize = maGetScrSize( );
		int width = EXTENT_X( screenSize );
		int height = EXTENT_Y( screenSize );
		//
		// Map widget
		//
		mMap = newobject( MapWidget, new MapWidget( 0, 0, width, height, NULL ) );
		mMap->enterMapUpdateScope( );
		mMap->setCenterPosition( LonLat( 18.07, 59.33 ) );
		mMap->setMagnification( 10 );
		mMap->exitMapUpdateScope( true );
		mMapSourceKind = MapSourceKind_OpenStreetMap;
		Font* font = newobject( Font, new Font( RES_FONT_VERDANA13BLACK ) );
		mMap->setFont( font );

		setMain( mMap );
		//
		// Have to wait until we have proper width and height
		//
		mMap->setMapSourceKind( mMapSourceKind );
	}

	//-------------------------------------------------------------------------
	HelloMapScreen::~HelloMapScreen( )
	//-------------------------------------------------------------------------
	{
		deleteobject( mMap );
	}

	//-------------------------------------------------------------------------
	void HelloMapScreen::keyPressEvent( int keyCode, int nativeCode )
	//-------------------------------------------------------------------------
	{
		switch( keyCode )
		{
		case MAK_2:
			nextMapSource( );
			return;
		case MAK_SOFTRIGHT:
			maExit( 0 );
			return;
		}
		
		(void)mMap->handleKeyPress( keyCode );

	}

	//-------------------------------------------------------------------------
	void HelloMapScreen::keyReleaseEvent( int keyCode, int nativeCode )
	//-------------------------------------------------------------------------
	{
		(void)mMap->handleKeyRelease( keyCode );
	}



	bool scrolling = false;
	int prevX;
	int prevY;

	int lastPointerPress = -1;

	//-------------------------------------------------------------------------
	void HelloMapScreen::pointerPressEvent(MAPoint2d p)
	//-------------------------------------------------------------------------
	{
		prevX = p.x;
		prevY = p.y;

		int curTime = maGetMilliSecondCount();
		int deltaTime = curTime - lastPointerPress;

		if(deltaTime < 200) {
			maExit(0);
		} else {
			lastPointerPress = curTime;
		}
	}

	//-------------------------------------------------------------------------
	void HelloMapScreen::pointerMoveEvent(MAPoint2d p)
	//-------------------------------------------------------------------------
	{
		if(scrolling) return;
		int dx = p.x - prevX;
		int dy = p.y - prevY;

		if(abs(dx) > abs(dy)) {
			// mostly moving horizontally
			if(dx < 0)
				mMap->scroll(SCROLLDIRECTION_EAST, false);
			else
				mMap->scroll(SCROLLDIRECTION_WEST, false);
		} else {
			// mostly moving vertically
			if(dy < 0)
				mMap->scroll(SCROLLDIRECTION_SOUTH, false);
			else
				mMap->scroll(SCROLLDIRECTION_NORTH, false);
		}
		prevX = p.x;
		prevY = p.y;
	}

	//-------------------------------------------------------------------------
	void HelloMapScreen::pointerReleaseEvent(MAPoint2d p)
	//-------------------------------------------------------------------------
	{
		scrolling = false;
	}


	//-------------------------------------------------------------------------
	void HelloMapScreen::nextMapSource( )
	//-------------------------------------------------------------------------
	{
		MapSourceKind newKind = (MapSourceKind)(mMapSourceKind + 1);
		if( newKind >= MapSourceKind_Last )
			newKind = (MapSourceKind)( MapSourceKind_None + 1 );
		mMapSourceKind = newKind;
		mMap->setMapSourceKind( newKind );
	}
}
