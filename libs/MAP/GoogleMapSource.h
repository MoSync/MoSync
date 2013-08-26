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
* \file GoogleMapSource.h
* \brief Map source for Google map tile servers
* \author Lars-Åke Vinberg
*/

#ifndef GOOGLEMAPSOURCE_H_
#define GOOGLEMAPSOURCE_H_

#include "MapSource.h"

namespace MAP
{
	class MapTile;
	class MapSourceListener;

	//=========================================================================
	/**
	 * \brief Enumerates variations of google map rendering.
	 */
	enum GoogleMapKind
	//=========================================================================
	{
		GoogleMapKind_None,
		/**
		 * Street map.
		 */
		GoogleMapKind_StreetMap,
		/**
		 * Aerial/satellite photography
		 */
		GoogleMapKind_Aerial,
		/*
		 * Aerial with map labels drawn on top.
		 */
		GoogleMapKind_Hybrid,
		GoogleMapKind_Last
	};

	//=========================================================================
	/**
	 * \brief Implements a MapSource for Google Static Maps.
	 *
	 * NOTE: This code is just a sample, 
	 * Google's Static Maps service is not intended for use as a map tile server.
	 */
	class GoogleMapSource : public MapSource
	//=========================================================================
	{
	public:
		/**
		 * Google Static Maps reqires a registered API key.
		 * see http://code.google.com/apis/maps/documentation/staticmaps/
		 * for details.
		 *
		 * NOTE: Assign your registered key string to GoogleMapSource::ApiKey before
		 * using this map source.
		 */
		static const char* ApiKey;
		/**
		 * Creates a GoogleMapSource of the specified map kind.
		 */
		GoogleMapSource( GoogleMapKind mapKind );
		virtual	~GoogleMapSource( );

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
			return 17; 
		}

		void getTileUrl( char* buffer, MapTileCoordinate tileXY );

		MapTileCoordinate lonLatToTile( LonLat lonlat, MagnificationType magnification );

		PixelCoordinate	lonLatToPixel( LonLat lonlat, MagnificationType magnification );
		
		LonLat tileCenterToLonLat( const int tileSize, const MapTileCoordinate& tile, const double offsetX, const double offsetY );
	
	private:
		GoogleMapKind mMapKind;
	};
}

#endif // GOOGLEMAPSOURCE_H_
