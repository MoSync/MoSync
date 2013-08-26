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
