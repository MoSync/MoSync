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
//#include "ExitAction.h"
//#include "MapLocationAction.h"
//#include "AppStyleMgr.h"
#include "MAHeaders.h"

namespace HelloMap
{
	//static const bool Trace = false;

	////-------------------------------------------------------------------------
	//static const char* MapSourceKindToString( MapSourceKind kind )
	////-------------------------------------------------------------------------
	//{
	//	switch( kind )
	//	{
	//	case MapSourceKind_OpenStreetMap: return "OpenStreetMap";
	//	case MapSourceKind_GoogleMap: return "Google (map)";
	//	case MapSourceKind_GoogleAerial: return "Google (aerial)";
	//	case MapSourceKind_GoogleHybrid: return "Google (hybrid)";
	//	case MapSourceKind_CloudMade1: return "CloudMade (type 1)";
	//	case MapSourceKind_CloudMade7: return "CloudMade (type 7)";
	//	//case MapSourceKind_VirtualEarth: return "Virtual Earth";
	//	default: 
	//		return "Unknown map type";
	//	}
	//}

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
		//MapSourceMgr* mgr = MapSourceMgr::get( );
		Font* font = newobject( Font, new Font( RES_FONT_VERDANA13BLACK ) );
		mMap->setFont( font );

		//setClientWidget( mMap );
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

	////-------------------------------------------------------------------------
	//bool HelloMapScreen::handleKeyPress( int keyCode )
	////-------------------------------------------------------------------------
	//{
	//	switch( keyCode )
	//	{
	//		case MAK_2:
	//			nextMapSource( );
	//			return true;
	//	}
	//	if ( AppScreen::handleKeyPress( keyCode ) )
	//		return true;

	//	return mMap->handleKeyPress( keyCode );
	//}

	////-------------------------------------------------------------------------
	//bool HelloMapScreen::handleKeyRelease( int keyCode )
	////-------------------------------------------------------------------------
	//{
	//	if ( AppScreen::handleKeyRelease( keyCode ) )
	//		return true;

	//	return mMap->handleKeyRelease( keyCode );
	//}

	//-------------------------------------------------------------------------
	void HelloMapScreen::keyPressEvent( int keyCode )
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
	void HelloMapScreen::keyReleaseEvent( int keyCode )
	//-------------------------------------------------------------------------
	{
		(void)mMap->handleKeyRelease( keyCode );
	}

	//-------------------------------------------------------------------------
	void HelloMapScreen::nextMapSource( )
	//-------------------------------------------------------------------------
	{
		MapSourceMgr* mgr = MapSourceMgr::get( );
		MapSourceKind newKind = (MapSourceKind)(mMapSourceKind + 1);
		if( newKind >= MapSourceKind_Last )
			newKind = (MapSourceKind)( MapSourceKind_None + 1 );
		mMapSourceKind = newKind;
		mMap->setMapSourceKind( newKind );
		//MessageMgr::get( )->postMessage( MapSourceKindToString( newKind ) );
	}

	////-------------------------------------------------------------------------
	//void HelloMapScreen::enumerateActions( Vector<Action*>& list )
	////-------------------------------------------------------------------------
	//{
	//	// add my actions
	//	list.add( newobject( MapLocationAction, new MapLocationAction( mMap, LonLat( 18.07, 59.33 ), "Stockholm" ) ) );
	//	list.add( newobject( ExitAction, new ExitAction( mMoblet ) ) );
	//}
}
