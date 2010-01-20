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

#include "MapDemoMoblet.h"
#include "MapSourceMgr.h"
#include "CloudMadeMapSource.h"
#include "GoogleMapSource.h"
#include "config.h"

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
		// Map keys are defined in config.h
		// Please copy config.h.template to config.h, register your own keys
		// and enter in config.h
		//
		CloudMadeMapSource::ApiKey = CLOUDMADE_API_KEY;
		GoogleMapSource::ApiKey = GOOGLE_API_KEY;
		
		mScreen = newobject( MapDemoScreen, new MapDemoScreen( (MobletEx*)this ) );
		mScreen->show( );
	}

	//-------------------------------------------------------------------------
	MapDemoMoblet::~MapDemoMoblet( )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	void MapDemoMoblet::shutdown( )
	//-------------------------------------------------------------------------
	{
		deleteobject( mScreen );
		MapCache::shutdown( );
		MapSourceMgr::shutdown( );
		delete this;
		maExit( 0 );
	}
}
