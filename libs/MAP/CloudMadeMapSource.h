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
* \file CloudMadeMapSource.h
* \brief Map source for CloudMade map tile servers
* \author Lars-Åke Vinberg
*/

#ifndef CLOUDMADEMAPSOURCE_H_
#define CLOUDMADEMAPSOURCE_H_

#include "MapSource.h"



namespace MAP
{
	using namespace MAUtil;

	class MapTile;
	class MapSourceListener;
	class MapSourceImageDownloader;

	/**
	 * \brief A map source using map tile servers at CloudMade (cloudmade.com). 
	 *
	 * CloudMade requires the developer to register to get an API key.
	 * See http://www.cloudmade.com for more details.
	 */
	class CloudMadeMapSource : public MapSource
	//=========================================================================
	{
	public:
		/**
		 * API key for use in calls to CloudMade tile servers.
		 * See cloudmade.com for more details.
		 */
		static const char* ApiKey;

	public:
		CloudMadeMapSource( int style ) 
			: MapSource( ), 
			  mStyle( style ) 
		{
		}
		/**
		 * Returns tile size in pixels.
		 */
		int getTileSize( ) const
		{
			//
			// CloudMade tile size is fixed at 256x256 pixels.
			//
			return 256; 
		}

		int getMagnificationMin( ) const
		{
			//
			// CloudMade smallest zoom level is 1, which translates
			// to 2 x 2 tiles for the entire world.
			return 1;
		}

		int getMagnificationMax( ) const
		{
			//
			// ClodMade largest zoom level is 17 as of June 2009.
			//
			return 17;
		}
		/**
		 * Returns style setting for the CloudMade map source.
		 */
		int getStyle( ) const
		{
			return mStyle;
		}
		/**
		 * Sets style setting for the CloudMade map source.
		 * Refer to CloudMade documentation for more information.
		 */
		void setStyle( int style )
		{
			mStyle = style;
		}
		/**
		 * Creates a URL for the specified tile coordinate.
		 */
		void getTileUrl( char* buffer, MapTileCoordinate tileXY );
		/**
		 * Converts a LonLat coordinate to a tile coordinate.
		 */
		MapTileCoordinate lonLatToTile( LonLat lonlat, MagnificationType magnification );
		/**
		 * Converts a LonLat coordinate to a global pixel coordinate.
		 */
		PixelCoordinate lonLatToPixel( LonLat lonlat, MagnificationType magnification );
		/**
		 * Convert tile center plus pixel offset to LonLat. 
		 */
		LonLat tileCenterToLonLat( const int tileSize, const MapTileCoordinate& tile, const double offsetX, const double offsetY );
		
	private:
		int mStyle;
	};
}

#endif // CLOUDMADEMAPSOURCE_H_
