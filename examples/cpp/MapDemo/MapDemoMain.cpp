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
