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

#ifndef GOOGLEMAPSOURCE_H_
#define GOOGLEMAPSOURCE_H_

#include "MapSource.h"

using namespace MAUtil;

namespace MAP
{
	class MapTile;
	class MapSourceListener;

	//=========================================================================
	enum GoogleMapsMapKind
	//=========================================================================
	{
		GoogleMapsMapKind_None,
		GoogleMapsMapKind_StreetMap,
		GoogleMapsMapKind_Aerial,
		GoogleMapsMapKind_Hybrid,
		GoogleMapsMapKind_Last
	};

	//=========================================================================
	class GoogleMapSource : public MapSource
	//=========================================================================
	{
	public:
		static const char*	ApiKey;

								GoogleMapSource( GoogleMapsMapKind mapKind );
		virtual					~GoogleMapSource( );
		//
		// MapSource overrides
		//
		MapSourceKind getSourceKind( ) const		
		{ 
			switch( mMapKind )
			{
			case GoogleMapsMapKind_StreetMap: return MapSourceKind_GoogleMap; 
			case GoogleMapsMapKind_Aerial: return MapSourceKind_GoogleAerial;
			case GoogleMapsMapKind_Hybrid: return MapSourceKind_GoogleHybrid;
			default: return (MapSourceKind)-1;
			}
		}

		MAExtent				getTileSize( ) const { return 256; }
		int						getMagnificationMin( ) const { return 1; }
		int						getMagnificationMax( ) const { return 17; }
		void					getTileUrl( char* buffer, MapTileCoordinate tileXY );
		MapTileCoordinate		lonLatToTile( LonLat lonlat, int magnification );
		PixelCoordinate			lonLatToPixel( LonLat lonlat, int magnification );
		LonLat					tileCenterToLonLat( const int tileSize, const MapTileCoordinate& tile, const double offsetX, const double offsetY );
	private:
		GoogleMapsMapKind		mMapKind;
	};
}

#endif // GOOGLEMAPSOURCE_H_
