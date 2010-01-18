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

#include "MaplibDemoMoblet.h"
#include "MapSourceMgr.h"
#include "CloudMadeMapSource.h"
#include "GoogleMapSource.h"

using namespace MAUtil;
using namespace Util;

namespace MaplibDemo
{
	static const bool Trace = false;

	const int BottomHeight = 25;
	//
	// Registered map API keys, please GET YOUR OWN.
	//



	//-------------------------------------------------------------------------
	MaplibDemoMoblet::MaplibDemoMoblet( )
		: mScreen( NULL )
	//-------------------------------------------------------------------------
	{
		//if ( Trace ) trace( );

		CloudMadeMapSource::ApiKey = "2d80ddd260155a7ba03e123176011fa4"; // registered on lars@8x10.se
		GoogleMapSource::ApiKey = "ABQIAAAASy5nCpi0Rkp7zjf61gof0RSwGmTQ8vMzg6QxXdllR2ZPmPu02RTSpCw5ldHGcP6DsMziSwGx8muqHQ"; // reg to lars@8x10.se
		mScreen = newobject( MaplibDemoScreen, new MaplibDemoScreen( (MobletEx*)this ) );
		mScreen->show( );
	}

	//-------------------------------------------------------------------------
	MaplibDemoMoblet::~MaplibDemoMoblet( )
	//-------------------------------------------------------------------------
	{
		//shutdown( );
	}

	//-------------------------------------------------------------------------
	void MaplibDemoMoblet::shutdown( )
	//-------------------------------------------------------------------------
	{
		deleteobject( mScreen );
		//AppStyleMgr::shutdown( );
		MapCache::shutdown( );
		MapSourceMgr::shutdown( );
		//MessageMgr::shutdown( );
		delete this;
		//MemoryMgr::dump( );
		maExit( 0 );
	}
}
