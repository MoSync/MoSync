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

#include <conprint.h>

#include "AppStyleMgr.h"
#include "MapDemoAppStyle.h"
#include "MapDemoMoblet.h"

//-------------------------------------------------------------------------
extern "C" int MAMain( )
//-------------------------------------------------------------------------
{
	// If you should encounter memory problems, you can
	// experiment with the size of the tile cache.
	// Set the cache size like this:
	//MapCache::get()->setCapacity(30);

	//
	// Init app style
	//
	AppStyleMgr::setStyle(
		newobject(MapDemo::MapDemoAppStyle, new MapDemo::MapDemoAppStyle()));
	MapDemo::MapDemoMoblet* moblet =
		newobject(MapDemo::MapDemoMoblet, new MapDemo::MapDemoMoblet());
	
	Moblet::run( moblet );

	deleteobject( moblet );
	MapCache::shutdown( );
	AppStyleMgr::shutdown( );
	MessageMgr::shutdown( );

	MemoryMgr::dump( );

	return 0;
}
