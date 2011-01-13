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

#include "HelloMapMoblet.h"
#include <MAP/CloudMadeMapSource.h>
#include <MAP/GoogleMapSource.h>

using namespace MAUtil;

namespace HelloMap
{
	//-------------------------------------------------------------------------
	HelloMapMoblet::HelloMapMoblet( )
		: mScreen( NULL )
	//-------------------------------------------------------------------------
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

		mScreen = newobject( HelloMapScreen, new HelloMapScreen( ) );
		mScreen->show( );
	}

	//-------------------------------------------------------------------------
	HelloMapMoblet::~HelloMapMoblet( )
	//-------------------------------------------------------------------------
	{
		deleteobject( mScreen );
	}
}
