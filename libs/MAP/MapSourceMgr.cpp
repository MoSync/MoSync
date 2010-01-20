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
	MapSourceMgr* MapSourceMgr::sSingleton = NULL;

	MapSourceMgr* MapSourceMgr::get( ) 
	{ 
		if ( sSingleton == NULL ) 
		{ 
			sSingleton = newobject( MapSourceMgr, new MapSourceMgr( ) );
		} 
		return sSingleton; 
	}

	void MapSourceMgr::shutdown( ) 
	{ 
		deleteobject( sSingleton );
	}

	//-------------------------------------------------------------------------
	MapSourceMgr::MapSourceMgr( ) :
	//-------------------------------------------------------------------------
		mMap( )
	{
		for ( int i = 0; i < MapSourceKind_Last; i++ )
			mMap[(MapSourceKind)i] = NULL;

		mMap[MapSourceKind_OpenStreetMap] = newobject( OpenStreetMapSource, new OpenStreetMapSource( ) );
		mMap[MapSourceKind_GoogleMap] = newobject( GoogleMapSource, new GoogleMapSource( GoogleMapsMapKind_StreetMap ) );
		mMap[MapSourceKind_GoogleAerial] = newobject( GoogleMapSource, new GoogleMapSource( GoogleMapsMapKind_Aerial ) );
		mMap[MapSourceKind_GoogleHybrid] = newobject( GoogleMapSource, new GoogleMapSource( GoogleMapsMapKind_Hybrid ) );
		mMap[MapSourceKind_CloudMade1] = newobject( CloudMadeMapSource, new CloudMadeMapSource( 1 ) );
		mMap[MapSourceKind_CloudMade7] = newobject( CloudMadeMapSource, new CloudMadeMapSource( 7 ) );
		//mMap[MapSourceKind_VirtualEarth] = newobject( VirtualEarthMapSource, new VirtualEarthMapSource( ) );
	}

	//-------------------------------------------------------------------------
	MapSourceMgr::~MapSourceMgr( )
	//-------------------------------------------------------------------------
	{
		//int count = mMap. mMap.getKeySet( ).size( );
		//for ( int i = 0; i < count; i++ )
		//{
		//	MapSourceKind kind = mMap.getKeySet( )[i];
		//	MapSource* source = mMap[ kind ];
		//	deleteobject( source );
		//}

		//
		// LAV 2009-10-21 The modified Map class is a mess.
		// we're skipping deallocation for that reason,
		// will leak some memory blocks on exit.
		//
		//for ( Dictionary<MapSourceKind, Pair<MapSourceKind, MapSource*> >::Iterator i = mMap.begin();
		//		i != NULL;
		//		i++ )
		//{
		//}

	}

	//-------------------------------------------------------------------------
	MapSource* MapSourceMgr::getMapSource( MapSourceKind kind )
	//-------------------------------------------------------------------------
	{
		return mMap[ kind ];
	}
}

