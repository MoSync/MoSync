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

#include "HelloMapScreen.h"
#include "MAHeaders.h"
#include <mastdlib.h>

#include <MAP/OpenStreetMapSource.h>
#include <MAP/GoogleMapSource.h>
#include "HelloMapMoblet.h"
#include <MAP/DebugPrintf.h>

namespace HelloMap
{
	static const int DoubleClickTimeMs = 200;
	static const int TapPanAcceleration = 1;
	static const int TapPanIntervalMs = 100;

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
		// Viewport
		//
		MapViewport* viewport = newobject( MapViewport, new MapViewport( ) );
		//
		// Map widget
		//
		mMap = newobject( MapWidget, new MapWidget( 0, 0, width, height, NULL ) );
		mMap->setViewport( viewport );

		mFont = newobject( Font, new Font( RES_FONT_VERDANA13BLACK ) );
		mMap->setFont( mFont );
		//mMap->setPanMode( MapViewportPanMode_Momentum );
		//
		// Create map sources
		//
		mOpenStreetMapSource = newobject( OpenStreetMapSource, new OpenStreetMapSource( ) );
		mGoogleStreetMapSource = newobject( GoogleMapSource, new GoogleMapSource( GoogleMapKind_StreetMap ) );
		mGoogleAerialMapSource = newobject( GoogleMapSource, new GoogleMapSource( GoogleMapKind_Aerial ) );
		mGoogleHybridMapSource = newobject( GoogleMapSource, new GoogleMapSource( GoogleMapKind_Hybrid ) );
		//
		// set map source
		//
		mMap->setMapSource( mOpenStreetMapSource );

		//
		// Stockholm (Must be set after the source has been set)
		//
		mMap->setCenterPosition( LonLat( 18.07, 59.33 ), 10, true, false );

		//
		// commit widget
		//
		setMain( mMap );
	}

	//-------------------------------------------------------------------------
	HelloMapScreen::~HelloMapScreen( )
	//-------------------------------------------------------------------------
	{
		deleteobject( mMap );
		deleteobject( mOpenStreetMapSource );
		deleteobject( mGoogleStreetMapSource );
		deleteobject( mGoogleAerialMapSource );
		deleteobject( mGoogleHybridMapSource );
		deleteobject( mFont );
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
		case MAK_8:
			MemoryMgr::dump( );
			return;
		//case MAK_7:
		//	mMap->stressTest( );
		//	return;
		case MAK_BACK:
			maExit(0);
			return;
		case MAK_SOFTRIGHT:
			HelloMapMoblet& moblet = (HelloMapMoblet&)Environment::getEnvironment( );
			moblet.Terminate( );
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

	int pointerPressTimeMs = -1;

	//-------------------------------------------------------------------------
	void HelloMapScreen::pointerPressEvent( MAPoint2d p )
	//-------------------------------------------------------------------------
	{
		//
		// Set up tap panning
		//
		mMap->getViewport()->beginPanning( p );

		//
		// Handle doubletap for app exit
		//
		int curTimeMs = maGetMilliSecondCount( );
		int deltaTimeMs = curTimeMs - pointerPressTimeMs;
		if ( deltaTimeMs < DoubleClickTimeMs )
		{
			HelloMapMoblet& moblet = (HelloMapMoblet&)Environment::getEnvironment( );
			moblet.Terminate( );
			//mMap->stressTest( );
		}
		else
		{
			pointerPressTimeMs = curTimeMs;
		}
	}

	int lastPointerMoveMs = -1;

	//-------------------------------------------------------------------------
	void HelloMapScreen::pointerMoveEvent( MAPoint2d p )
	//-------------------------------------------------------------------------
	{
		mMap->getViewport( )->updatePanning( p );

		int curTimeMs = maGetMilliSecondCount( );
		if ( curTimeMs < lastPointerMoveMs + TapPanIntervalMs )
			return;
	}

	//-------------------------------------------------------------------------
	void HelloMapScreen::pointerReleaseEvent( MAPoint2d p )
	//-------------------------------------------------------------------------
	{
		mMap->getViewport( )->endPanning( );
	}

	//-------------------------------------------------------------------------
	void HelloMapScreen::nextMapSource( )
	//-------------------------------------------------------------------------
	{
		MapSource* current = mMap->getMapSource( );
		if ( current == mOpenStreetMapSource )
			mMap->setMapSource( mGoogleStreetMapSource );
		else if ( current == mGoogleStreetMapSource )
			mMap->setMapSource( mGoogleAerialMapSource );
		else if ( current == mGoogleAerialMapSource )
			mMap->setMapSource( mGoogleHybridMapSource );
		else if ( current == mGoogleHybridMapSource )
			mMap->setMapSource( mOpenStreetMapSource );
	}
}
