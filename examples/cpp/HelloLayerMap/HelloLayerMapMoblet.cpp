/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "HelloLayerMapMoblet.h"
#include <MAP/CloudMadeMapSource.h>
#include <MAP/GoogleMapSource.h>

using namespace MAUtil;

namespace HelloLayerMap
{
	//-------------------------------------------------------------------------
	HelloLayerMapMoblet::HelloLayerMapMoblet( )
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


		mScreen = newobject( HelloLayerMapScreen, new HelloLayerMapScreen( ) );
		mScreen->show( );
	}

	//-------------------------------------------------------------------------
	HelloLayerMapMoblet::~HelloLayerMapMoblet( )
	//-------------------------------------------------------------------------
	{
		deleteobject( mScreen );
	}
}
