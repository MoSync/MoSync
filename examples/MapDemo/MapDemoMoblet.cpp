/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

#include "MapDemoMoblet.h"
#include <MAP/CloudMadeMapSource.h>
#include <MAP/GoogleMapSource.h>

using namespace MAUtil;
using namespace MapDemoUtil;

namespace MapDemo
{
	const int BottomHeight = 25;

	//-------------------------------------------------------------------------
	MapDemoMoblet::MapDemoMoblet( )
	//-------------------------------------------------------------------------
		: mScreen( NULL )
	{
		//
		// CloudMade map key
		// Used to access CloudMade map tile servers.
		// Please register with CloudMade to get a personal API key at
		// http://cloudmade.com/
		//
		#ifdef CLOUDMADE_API_KEY
		CloudMadeMapSource::ApiKey = CLOUDMADE_API_KEY;
		#endif

		//
		// Google Static Maps API key
		// Used to access Google Static Maps tile servers.
		// Please register with Google to get a personal API key at
		// http://code.google.com/apis/maps/documentation/staticmaps/
		//
		#ifdef GOOGLE_API_KEY
		GoogleMapSource::ApiKey = GOOGLE_API_KEY;
		#endif

		
		mScreen = newobject( MapDemoScreen, new MapDemoScreen( (MobletEx*)this ) );
		mScreen->show( );
	}

	//-------------------------------------------------------------------------
	MapDemoMoblet::~MapDemoMoblet( )
	//-------------------------------------------------------------------------
	{
		deleteobject( mScreen );
	}

	//-------------------------------------------------------------------------
	void MapDemoMoblet::shutdown( )
	//-------------------------------------------------------------------------
	{
		Terminate( );
	}
}
