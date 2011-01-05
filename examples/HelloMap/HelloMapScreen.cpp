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

#include <MAP/OpenStreetMapSource.h>
#include <MAP/GoogleMapSource.h>
#include "HelloMapMoblet.h"
#include <MAP/DebugPrintf.h>

namespace HelloMap
{
	static const int DoubleClickTimeMs = 200;
	static const int TapPanAcceleration = 1;
	static const int TapPanIntervalMs = 80;

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
		mMap = newobject( MapWidget, new MapWidget( /*mMapSource,*/ 0, 0, width, height, NULL ) );
		mMap->enterMapUpdateScope( );
		mMap->setCenterPosition( LonLat( 18.07, 59.33 ) );
		mMap->setMagnification( 10 );
		mMap->exitMapUpdateScope( true );
		mFont = newobject( Font, new Font( RES_FONT_VERDANA13BLACK ) );
		mMap->setFont( mFont );
		mMap->setPanMode( MapWidgetPanMode_Momentum );
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
		case MAK_SOFTRIGHT:
			//maExit( 0 );
			// TODO: proper exit
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



	bool scrolling = false;
	//int prevX;
	//int prevY;
	//int lastPointerPressTimeMs = -1;

	int pointerPressX;
	int pointerPressY;
	PixelCoordinate pointerPressCenter;
	int pointerPressTimeMs = -1;

	//-------------------------------------------------------------------------
	void HelloMapScreen::pointerPressEvent( MAPoint2d p )
	//-------------------------------------------------------------------------
	{
		//
		// Set up tap panning
		//
		//prevX = p.x;
		//prevY = p.y;
		pointerPressX = p.x;
		pointerPressY = p.y;
		pointerPressCenter = mMap->getCenterPositionPixels( );
		//
		// Handle doubletap for app exit
		//
		int curTimeMs = maGetMilliSecondCount( );
		int deltaTimeMs = curTimeMs - pointerPressTimeMs;
		if ( deltaTimeMs < DoubleClickTimeMs ) 
		{
			//maExit( 0 );
			HelloMapMoblet& moblet = (HelloMapMoblet&)Environment::getEnvironment( );
			moblet.Terminate( );
		}
		else
		{
			pointerPressTimeMs = curTimeMs;
		}
	}

    //-------------------------------------------------------------------------
    static double clamp( double x, double min, double max )
    //-------------------------------------------------------------------------
    {
            return x < min ? min : x > max ? max : x;
    }

	int lastPointerMoveMs = -1;
	int lastPointerMoveX;
	int lastPointerMoveY;
	int lastPointerSpeedX;
	int lastPointerSpeedY;

	//-------------------------------------------------------------------------
	void HelloMapScreen::pointerMoveEvent( MAPoint2d p )
	//-------------------------------------------------------------------------
	{
		if( scrolling ) 
			return;

		int curTimeMs = maGetMilliSecondCount( );
		if ( curTimeMs < lastPointerMoveMs + TapPanIntervalMs )
			return;
		//
		// Calculate pointer speed
		//
		int deltaX = p.x - lastPointerMoveX;
		int deltaY = p.y - lastPointerMoveY;
		int deltaMs = curTimeMs - lastPointerMoveMs;
		lastPointerSpeedX = deltaX * 1000 / deltaMs; 
		lastPointerSpeedY = deltaY * 1000 / deltaMs;
		DebugPrintf( "deltaMs = %d, delta = %d, %d\n", deltaMs, deltaX, deltaY );

		lastPointerMoveMs = curTimeMs;
		lastPointerMoveX = p.x;
		lastPointerMoveY = p.y;

		//int accel = 2;

		//int dx = ( p.x - prevX ) * accel;
		//int dy = ( p.y - prevY ) * accel;
		//PixelCoordinate px = mMap->getCenterPositionPixels( );
		//px = PixelCoordinate( px.getMagnification( ), px.getX( ) - dx, px.getY( ) + dy );
		int dx = ( p.x - pointerPressX ) * TapPanAcceleration;
		int dy = ( p.y - pointerPressY ) * TapPanAcceleration;
		PixelCoordinate px = PixelCoordinate(	pointerPressCenter.getMagnification( ), 
												pointerPressCenter.getX( ) - dx,
												pointerPressCenter.getY( ) + dy );
        LonLat newPos = LonLat( px );
        newPos = LonLat( clamp( newPos.lon, -180, 180 ), clamp( newPos.lat, -85, 85 ) );
        mMap->setCenterPosition( newPos );

		//prevX = p.x;
		//prevY = p.y;
	}

	//-------------------------------------------------------------------------
	void HelloMapScreen::pointerReleaseEvent(MAPoint2d p)
	//-------------------------------------------------------------------------
	{
		scrolling = false;
		//
		// Set up for momentum-based panning
		//
		//int curTimeMs = maGetMilliSecondCount( );
		//int deltaMs = curTimeMs - /*lastPointerMoveMs*/pointerPressTimeMs;
		//int deltaX = p.x - /*lastPointerMoveX*/pointerPressX;
		//int deltaY = p.y - /*lastPointerMoveY*/pointerPressY;
		//if ( deltaMs >0 ) mMap->setPanMomentum( deltaX * 1000 / deltaMs, deltaY * 1000 / deltaMs );
	
		DebugPrintf( "momentum = %d, %d\n", lastPointerSpeedX, lastPointerSpeedY );
		mMap->setPanMomentum( lastPointerSpeedX, lastPointerSpeedY );
	}


	//-------------------------------------------------------------------------
	void HelloMapScreen::nextMapSource( )
	//-------------------------------------------------------------------------
	{
		return;
		// TODO: Reimplement later
	}
}
