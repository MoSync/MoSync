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

#include "MemoryMgr.h"
#include "MapSourceMgr.h"
#include "OpenStreetMapSource.h"
#include "GoogleMapSource.h"
#include "CloudMadeMapSource.h"

namespace MAP
{
	MapSourceMgr* MapSourceMgr::s_singleton = NULL;

	MapSourceMgr* MapSourceMgr::get( ) 
	{ 
		if ( s_singleton == NULL ) 
		{ 
			s_singleton = newobject( MapSourceMgr, new MapSourceMgr( ) );
		} 
		return s_singleton; 
	}

	void MapSourceMgr::shutdown( ) 
	{ 
		deleteobject( s_singleton );
	}

	//-------------------------------------------------------------------------
	MapSourceMgr::MapSourceMgr( ) :
	//-------------------------------------------------------------------------
		m_map( )
	{
		for ( int i = 0; i < MapSourceKind_Last; i++ )
			m_map[(MapSourceKind)i] = NULL;

		m_map[MapSourceKind_OpenStreetMap] = newobject( OpenStreetMapSource, new OpenStreetMapSource( ) );
		m_map[MapSourceKind_GoogleMap] = newobject( GoogleMapSource, new GoogleMapSource( GoogleMapsMapKind_StreetMap ) );
		m_map[MapSourceKind_GoogleAerial] = newobject( GoogleMapSource, new GoogleMapSource( GoogleMapsMapKind_Aerial ) );
		m_map[MapSourceKind_GoogleHybrid] = newobject( GoogleMapSource, new GoogleMapSource( GoogleMapsMapKind_Hybrid ) );
		m_map[MapSourceKind_CloudMade1] = newobject( CloudMadeMapSource, new CloudMadeMapSource( 1 ) );
		m_map[MapSourceKind_CloudMade7] = newobject( CloudMadeMapSource, new CloudMadeMapSource( 7 ) );
		//m_map[MapSourceKind_VirtualEarth] = newobject( VirtualEarthMapSource, new VirtualEarthMapSource( ) );
	}

	//-------------------------------------------------------------------------
	MapSourceMgr::~MapSourceMgr( )
	//-------------------------------------------------------------------------
	{
		//int count = m_map. m_map.getKeySet( ).size( );
		//for ( int i = 0; i < count; i++ )
		//{
		//	MapSourceKind kind = m_map.getKeySet( )[i];
		//	MapSource* source = m_map[ kind ];
		//	deleteobject( source );
		//}

		//
		// LAV 2009-10-21 The modified Map class is a mess.
		// we're skipping deallocation for that reason,
		// will leak some memory blocks on exit.
		//
		//for ( Dictionary<MapSourceKind, Pair<MapSourceKind, MapSource*> >::Iterator i = m_map.begin();
		//		i != NULL;
		//		i++ )
		//{
		//}

	}



	//-------------------------------------------------------------------------
	MapSource* MapSourceMgr::getMapSource( MapSourceKind kind )
	//-------------------------------------------------------------------------
	{
		return m_map[ kind ];
	}
}

