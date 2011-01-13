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

/** 
* \file CloudMadeMapSource.h
* \brief Map source for CloudMade map tile servers
* \author Lars-Åke Vinberg
*/

#ifndef CLOUDMADEMAPSOURCE_H_
#define CLOUDMADEMAPSOURCE_H_

#include "MapSource.h"

using namespace MAUtil;

namespace MAP
{
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
		MAExtent getTileSize( ) const
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
		MapTileCoordinate lonLatToTile( LonLat lonlat, int magnification );
		/**
		 * Converts a LonLat coordinate to a global pixel coordinate.
		 */
		PixelCoordinate lonLatToPixel( LonLat lonlat, int magnification );
		/**
		 * Convert tile center plus pixel offset to LonLat. 
		 */
		LonLat tileCenterToLonLat( const int tileSize, const MapTileCoordinate& tile, const double offsetX, const double offsetY );
		
	private:
		int mStyle;
	};
}

#endif // CLOUDMADEMAPSOURCE_H_
