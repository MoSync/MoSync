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

#include <mastdlib.h>

#include "MapDemoScreen.h"
#include "ExitAction.h"
#include "MapLocationAction.h"
#include "MapSourceAction.h"
#include "AppStyleMgr.h"

namespace MapDemo
{
	//-------------------------------------------------------------------------
	static const char* MapSourceKindToString( MapSourceKind kind )
	//-------------------------------------------------------------------------
	{
		switch( kind )
		{
		case MapSourceKind_OpenStreetMap: return "OpenStreetMap";
		case MapSourceKind_GoogleMap: return "Google (map)";
		case MapSourceKind_GoogleAerial: return "Google (aerial)";
		case MapSourceKind_GoogleHybrid: return "Google (hybrid)";
		case MapSourceKind_CloudMade1: return "CloudMade (type 1)";
		case MapSourceKind_CloudMade7: return "CloudMade (type 7)";
		//case MapSourceKind_VirtualEarth: return "Virtual Earth";
		default: 
			return "Unknown map type";
		}
	}

	//-------------------------------------------------------------------------
	MapDemoScreen::MapDemoScreen( MobletEx* moblet ) :
		AppScreen( moblet ),
		scrolling( false ),
		lastPointerPress( -1 )
	//-------------------------------------------------------------------------
	{
		//
		// Map widget
		//
		mMap = newobject( MapWidget, new MapWidget( 0, 0, 0, 0, NULL ) );
		mMap->enterMapUpdateScope( );
		mMap->setCenterPosition( LonLat( 18.07, 59.33 ) );
		mMap->setMagnification( 10 );
		mMap->exitMapUpdateScope( true );
		mMapSourceKind = MapSourceKind_OpenStreetMap;
		mMap->setFont( AppStyleMgr::getStyle( )->getFont( FontSize_Smallest, Color::black, false ) );

		setClientWidget( mMap );
		//
		// Have to wait until we have proper width and height
		//
		mMap->setMapSourceKind( mMapSourceKind );
	}

	//-------------------------------------------------------------------------
	MapDemoScreen::~MapDemoScreen( )
	//-------------------------------------------------------------------------
	{
		deleteobject( mMap );
	}

	//-------------------------------------------------------------------------
	bool MapDemoScreen::handleKeyPress( int keyCode )
	//-------------------------------------------------------------------------
	{
		switch( keyCode )
		{
			case MAK_2:
				nextMapSource( );
				return true;
		}
		if ( AppScreen::handleKeyPress( keyCode ) )
			return true;

		return mMap->handleKeyPress( keyCode );
	}

	//-------------------------------------------------------------------------
	bool MapDemoScreen::handleKeyRelease( int keyCode )
	//-------------------------------------------------------------------------
	{
		if ( AppScreen::handleKeyRelease( keyCode ) )
			return true;

		return mMap->handleKeyRelease( keyCode );
	}

	//-------------------------------------------------------------------------
	bool MapDemoScreen::handlePointerPress( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		if ( AppScreen::handlePointerPress( point ) )
			return true;

		prevX = point.x;
		prevY = point.y;

		int curTime = maGetMilliSecondCount();
		int deltaTime = curTime - lastPointerPress;

		if(deltaTime < 200)
		{
			maExit(0);
		}
		else
		{
			lastPointerPress = curTime;
		}
		return true;
	}

    //-------------------------------------------------------------------------
    static double clamp( double x, double min, double max )
    //-------------------------------------------------------------------------
    {
            return x < min ? min : x > max ? max : x;
    }

	//-------------------------------------------------------------------------
	bool MapDemoScreen::handlePointerMove( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		if ( AppScreen::handlePointerMove( point ) )
			return true;

		if(scrolling) return false;
		int accel = 7;
		int dx = (point.x - prevX) * accel;
		int dy = (point.y - prevY) * accel;

		PixelCoordinate px = mMap->getCenterPositionPixels( );

		px = PixelCoordinate( px.getMagnification( ), px.getX( ) - dx, px.getY( ) + dy );

        LonLat newPos = LonLat( px );
        newPos = LonLat( clamp( newPos.lon, -180, 180 ), clamp( newPos.lat, -85, 85 ) );
        mMap->setCenterPosition( newPos );
		prevX = point.x;
		prevY = point.y;
		return true;
	}

	//-------------------------------------------------------------------------
	bool MapDemoScreen::handlePointerRelease( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		if ( AppScreen::handlePointerRelease( point ) )
			return true;

		scrolling = false;
		return true;
	}

	//-------------------------------------------------------------------------
	void MapDemoScreen::nextMapSource( )
	//-------------------------------------------------------------------------
	{
		MapSourceKind newKind = (MapSourceKind)(mMapSourceKind + 1);
		if( newKind >= MapSourceKind_Last )
			newKind = (MapSourceKind)( MapSourceKind_None + 1 );
		mMapSourceKind = newKind;
		mMap->setMapSourceKind( newKind );
		MessageMgr::get( )->postMessage( MapSourceKindToString( newKind ) );
	}

	//-------------------------------------------------------------------------
	void MapDemoScreen::enumerateActions( Vector<Action*>& list )
	//-------------------------------------------------------------------------
	{
		// add my actions
		list.add( newobject( MapLocationAction, new MapLocationAction( mMap, LonLat( 18.07, 59.33 ), "Stockholm" ) ) );
		list.add( newobject( MapSourceAction, new MapSourceAction( mMap, mMapSourceKind, MapSourceKind_OpenStreetMap, "OpenStreetMap" ) ) );
		list.add( newobject( MapSourceAction, new MapSourceAction( mMap, mMapSourceKind, MapSourceKind_GoogleMap, "Google (map)" ) ) );
		list.add( newobject( MapSourceAction, new MapSourceAction( mMap, mMapSourceKind, MapSourceKind_GoogleAerial, "Google (aerial)" ) ) );
		list.add( newobject( MapSourceAction, new MapSourceAction( mMap, mMapSourceKind, MapSourceKind_GoogleHybrid, "Google (hybrid)" ) ) );
		//
		// Uncomment the following lines if you want to use CloudMade maps with your API key.
		// Without any key CloudMade maps will not be loaded.
		// See MapDemoMoblet.cpp for more information about the API Key.
		//
		//list.add( newobject( MapSourceAction, new MapSourceAction( mMap, mMapSourceKind, MapSourceKind_CloudMade1, "CloudMade (type 1)" ) ) );
		//list.add( newobject( MapSourceAction, new MapSourceAction( mMap, mMapSourceKind, MapSourceKind_CloudMade7, "CloudMade (type 7)" ) ) );
		list.add( newobject( ExitAction, new ExitAction( mMoblet ) ) );
	}
}
