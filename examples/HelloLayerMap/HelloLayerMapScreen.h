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

#ifndef HELLOMAPSCREEN_H_
#define HELLOMAPSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUtil/Moblet.h>
#include <MAP/MapWidget.h>
#include <MAP/GeoPointDataSource.h>
#include <MAP/GeoPointLayerRenderer.h>
#include <MAUI/Font.h>

using namespace MAP;
using namespace MAUI;

namespace HelloLayerMap 
{
	class TestDataSource;

	//=========================================================================
	//
	// Screen for HelloLayerMap app
	//
	class HelloLayerMapScreen : public Screen
	//=========================================================================
	{
	public:
		HelloLayerMapScreen( );
		virtual ~HelloLayerMapScreen( );
		//
		// Screen overrides
		//
		void keyPressEvent( int keyCode );
		void keyReleaseEvent( int keyCode );

		// Need this right now
		void multitouchPressEvent(MAPoint2d point, int touchId) {}
		void multitouchMoveEvent(MAPoint2d point, int touchId) {}
		void multitouchReleaseEvent(MAPoint2d point, int touchId) {}

	private:
		void nextMapSource( );
		void addPoint( );

		Font* mBlackFont;
		MapWidget* mMap;
		MapSource* mOpenStreetMapSource;
		MapSource* mGoogleStreetMapSource;
		MapSource* mGoogleAerialMapSource;
		MapSource* mGoogleHybridMapSource;
		TestDataSource* mDataSource;
		GeoPointLayerRenderer* mRenderer;
	};
}

#endif // HELLOMAPSCREEN_H_
