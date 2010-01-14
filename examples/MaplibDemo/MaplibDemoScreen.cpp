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

#include "MaplibDemoScreen.h"
#include "ExitAction.h"
#include "MapLocationAction.h"
#include "AppStyleMgr.h"
//#include "TraceScope.h"
//#include "MemoryMgr.h"

//using namespace Mopub;

namespace MaplibDemo
{
	static const bool Trace = false;

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
			//DebugAssert( false );
			return "Unknown map type";
		}
	}

	//-------------------------------------------------------------------------
	MaplibDemoScreen::MaplibDemoScreen( MobletEx* moblet ) :
		AppScreen( moblet )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		//
		// Map widget
		//
		m_map = newobject( MapWidget, new MapWidget( 0, 0, 0, 0, NULL ) );
		m_map->enterMapUpdateScope( );
		m_map->setCenterPosition( LonLat( 18.07, 59.33 ) );
		m_map->setMagnification( 10 );
		m_map->exitMapUpdateScope( true );
		m_mapSourceKind = MapSourceKind_OpenStreetMap;
		MapSourceMgr* mgr = MapSourceMgr::get( );
		m_map->setFont( AppStyleMgr::getStyle( )->getFont( FontSize_Smallest, Color::black, false ) );

		setClientWidget( m_map );
		//
		// Have to wait until we have proper width and height
		//
		m_map->setMapSourceKind( m_mapSourceKind );
	}

	//-------------------------------------------------------------------------
	MaplibDemoScreen::~MaplibDemoScreen( )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );
		
		deleteobject( m_map );
	}

	//-------------------------------------------------------------------------
	bool MaplibDemoScreen::handleKeyPress( int keyCode )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		switch( keyCode )
		{
			case MAK_2:
				nextMapSource( );
				return true;
		}
		if ( AppScreen::handleKeyPress( keyCode ) )
			return true;

		return m_map->handleKeyPress( keyCode );
	}

	//-------------------------------------------------------------------------
	bool MaplibDemoScreen::handleKeyRelease( int keyCode )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		if ( AppScreen::handleKeyRelease( keyCode ) )
			return true;

		return m_map->handleKeyRelease( keyCode );
	}

	//-------------------------------------------------------------------------
	void MaplibDemoScreen::nextMapSource( )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		MapSourceMgr* mgr = MapSourceMgr::get( );
		MapSourceKind newKind = (MapSourceKind)(m_mapSourceKind + 1);
		if( newKind >= MapSourceKind_Last )
			newKind = (MapSourceKind)( MapSourceKind_None + 1 );
		m_mapSourceKind = newKind;
		m_map->setMapSourceKind( newKind );
		MessageMgr::get( )->postMessage( MapSourceKindToString( newKind ) );
	}

	//-------------------------------------------------------------------------
	void MaplibDemoScreen::enumerateActions( Vector<Action*>& list )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		// add my actions
		list.add( newobject( MapLocationAction, new MapLocationAction( m_map, LonLat( 18.07, 59.33 ), "Stockholm" ) ) );
		list.add( newobject( ExitAction, new ExitAction( m_moblet ) ) );
	}
}
