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

//
// GeoPoint.cpp
//
// Author: Lars Ake Vinberg
//

#include "MapConfig.h"
#include "MemoryMgr.h"
#include "GeoPoint.h"
#include "PixelCoordinate.h"

namespace MAP
{
	//-------------------------------------------------------------------------
	GeoPoint::GeoPoint( ) :
	//-------------------------------------------------------------------------
		mLocation( ),
		mCachedWorldPixelLocation( ),
		mTitle( ),
		mDescription( ),
		mMarkerSize( ),
		mTimestamp( )
	{
	}

	//-------------------------------------------------------------------------
	GeoPoint::GeoPoint( LonLat location, const char* title, const char* description, int markerSize, const DateTime timestamp ) :
	//-------------------------------------------------------------------------
		mLocation( location ),
		mCachedWorldPixelLocation( ),
		mTitle( title ),
		mDescription( description ),
		mMarkerSize( markerSize ),
		mTimestamp( timestamp )
	{
	}

	//-------------------------------------------------------------------------
	GeoPoint::~GeoPoint( )
	//-------------------------------------------------------------------------
	{
	}
}
