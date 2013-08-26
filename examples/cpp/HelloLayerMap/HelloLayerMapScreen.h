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
		//
		// PointerListener overrides
		//
		void pointerPressEvent(MAPoint2d p);
		void pointerMoveEvent(MAPoint2d p);
		void pointerReleaseEvent(MAPoint2d p);

		// Need this right now
		void multitouchPressEvent(MAPoint2d point, int touchId) {}
		void multitouchMoveEvent(MAPoint2d point, int touchId) {}
		void multitouchReleaseEvent(MAPoint2d point, int touchId) {}

	private:
		void setPosition( MAPoint2d p );
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
