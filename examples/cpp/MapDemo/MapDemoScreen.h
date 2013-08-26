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

#ifndef MAPDEMOSCREEN_H_
#define MAPDEMOSCREEN_H_

#include <MAUI/Screen.h>
#include <MAUtil/Moblet.h>
#include "AppScreen.h"
#include <MAP/MapWidget.h>

using namespace MAP;
using namespace MapDemoUI;

namespace MapDemo
{
	//=========================================================================
	//
	// Screen for MapDemo app
	//
	class MapDemoScreen : public AppScreen
	//=========================================================================
	{
	public:
		MapDemoScreen( MobletEx* mMoblet );

		virtual ~MapDemoScreen( );

		//
		// Key handling
		//
		virtual bool handleKeyPress( int keyCode );
		virtual bool handleKeyRelease( int keyCode );
		//
		// Pointer handling
		//
		virtual bool handlePointerPress( MAPoint2d point );
		virtual bool handlePointerMove( MAPoint2d point );
		virtual bool handlePointerRelease( MAPoint2d point );
		virtual void multitouchPressEvent(MAPoint2d p, int touchId) { }
		virtual void multitouchMoveEvent(MAPoint2d p, int touchId) { }
		virtual void multitouchReleaseEvent(MAPoint2d p, int touchId) { }

		virtual void enumerateActions( Vector<Action*>& list );

	private:
		void nextMapSource( );

		MapWidget* mMap;
		MapSource* mOpenStreetMapSource;
		MapSource* mGoogleStreetMapSource;
		MapSource* mGoogleAerialMapSource;
		MapSource* mGoogleHybridMapSource;

		int lastPointerPress;
	};
}

#endif // MAPDEMOSCREEN_H_
