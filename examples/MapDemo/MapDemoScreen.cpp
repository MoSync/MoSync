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
#include <MAP/OpenStreetMapSource.h>
#include <MAP/GoogleMapSource.h>
#include "MapDemoMoblet.h"

namespace MapDemo
{
	//-------------------------------------------------------------------------
	MapDemoScreen::MapDemoScreen( MobletEx* moblet ) :
		AppScreen( moblet ),
		lastPointerPress( -1 )
	//-------------------------------------------------------------------------
	{
		MapViewport* viewport = newobject( MapViewport, new MapViewport( ) );
		//
		// Map widget
		//
		mMap = newobject( MapWidget, new MapWidget( 0, 0, 0, 0, NULL ) );
		mMap->setViewport( viewport );

		mMap->setFont( AppStyleMgr::getStyle( )->getFont( FontSize_Smallest, Color::black, false ) );
		setClientWidget( mMap );
		//
		// Create map sources
		//
		mOpenStreetMapSource = newobject( OpenStreetMapSource, new OpenStreetMapSource( ) );
		mGoogleStreetMapSource = newobject( GoogleMapSource, new GoogleMapSource( GoogleMapKind_StreetMap ) );
		mGoogleAerialMapSource = newobject( GoogleMapSource, new GoogleMapSource( GoogleMapKind_Aerial ) );
		mGoogleHybridMapSource = newobject( GoogleMapSource, new GoogleMapSource( GoogleMapKind_Hybrid ) );
		//
		// Have to wait until we have proper width and height
		//
		mMap->setMapSource( mOpenStreetMapSource );

		mMap->setCenterPosition( LonLat( 18.07, 59.33 ), 10, true, false );
	}

	//-------------------------------------------------------------------------
	MapDemoScreen::~MapDemoScreen( )
	//-------------------------------------------------------------------------
	{
		deleteobject( mMap );
		deleteobject( mOpenStreetMapSource );
		deleteobject( mGoogleStreetMapSource );
		deleteobject( mGoogleAerialMapSource );
		deleteobject( mGoogleHybridMapSource );
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
			case MAK_BACK:
			case MAK_SOFTRIGHT:
				//maExit( 0 );
				// TODO: proper exit
				MapDemoMoblet& moblet = (MapDemoMoblet&)Environment::getEnvironment( );
				moblet.Terminate( );
				return true;
		}

		if ( AppScreen::handleKeyPress( keyCode ) )
			return true;

		// Here we tell the map widget to handle the key press.
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

		mMap->getViewport( )->beginPanning( point );

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
	bool MapDemoScreen::handlePointerMove( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		if ( AppScreen::handlePointerMove( point ) )
			return true;

		mMap->getViewport( )->updatePanning( point );

		return true;
	}

	//-------------------------------------------------------------------------
	bool MapDemoScreen::handlePointerRelease( MAPoint2d point )
	//-------------------------------------------------------------------------
	{
		if ( AppScreen::handlePointerRelease( point ) )
			return true;

		mMap->getViewport( )->endPanning( );
		return true;
	}

	//-------------------------------------------------------------------------
	void MapDemoScreen::nextMapSource( )
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

	//-------------------------------------------------------------------------
	void MapDemoScreen::enumerateActions( Vector<Action*>& list )
	//-------------------------------------------------------------------------
	{
		// add my actions
		list.add( newobject( MapLocationAction, new MapLocationAction( mMap, LonLat( 18.07, 59.33 ), "Stockholm" ) ) );
		list.add( newobject( MapSourceAction, new MapSourceAction( mMap, mOpenStreetMapSource, "OpenStreetMap" ) ) );
		list.add( newobject( MapSourceAction, new MapSourceAction( mMap, mGoogleStreetMapSource, "Google (map)" ) ) );
		list.add( newobject( MapSourceAction, new MapSourceAction( mMap, mGoogleAerialMapSource, "Google (aerial)" ) ) );
		list.add( newobject( MapSourceAction, new MapSourceAction( mMap, mGoogleHybridMapSource, "Google (hybrid)" ) ) );

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
