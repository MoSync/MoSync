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

/** 
* \file OpenStreetMapSource.h
* \brief Map source for OpenStreet map tile servers
* \author Lars-Åke Vinberg
*/

#ifndef OPENSTREETMAPSOURCE_H_
#define OPENSTREETMAPSOURCE_H_

#include "MapSource.h"



namespace MAP
{
	using namespace MAUtil;
	
	class MapTile;
	class MapSourceListener;
	class MapSourceImageDownloader;

	/** 
	* \brief OpenStreetMap map source
	*/
	class OpenStreetMapSource : public MapSource
	//=========================================================================
	{
	public:
		OpenStreetMapSource( );
		
		virtual ~OpenStreetMapSource( );
		//
		// MapSource overrides
		//
		int getTileSize( ) const			
		{ 
			return 256; 
		}

		int getMagnificationMin( ) const	
		{ 
			return 1; 
		}

		int getMagnificationMax( ) const	
		{ 
			return 18; 
		}


		void getTileUrl( char* buffer, MapTileCoordinate tileXY );
		MapTileCoordinate lonLatToTile( LonLat lonlat, MagnificationType magnification );
		PixelCoordinate lonLatToPixel( LonLat lonlat, MagnificationType magnification );
		LonLat tileCenterToLonLat( const int tileSize, const MapTileCoordinate& tile, const double offsetX, const double offsetY );
	};
}

#endif // OPENSTREETMAPSOURCE_H_
