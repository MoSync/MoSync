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
* \file OpenStreetMapSource.h
* \brief Map source for OpenStreet map tile servers
* \author Lars-Åke Vinberg
*/

#ifndef OPENSTREETMAPSOURCE_H_
#define OPENSTREETMAPSOURCE_H_

#include "MapSource.h"

using namespace MAUtil;

namespace MAP
{
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
		MapTileCoordinate lonLatToTile( LonLat lonlat, int magnification );
		PixelCoordinate lonLatToPixel( LonLat lonlat, int magnification );
		LonLat tileCenterToLonLat( const int tileSize, const MapTileCoordinate& tile, const double offsetX, const double offsetY );
	};
}

#endif // OPENSTREETMAPSOURCE_H_
