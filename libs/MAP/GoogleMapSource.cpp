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

#include "MapConfig.h"
#include "GoogleMapSource.h"
#include "MapTileCoordinate.h"
#include <maassert.h>

namespace MAP
{
	const char* GoogleMapSource::ApiKey = "";

	//-------------------------------------------------------------------------
	//
	// Creates a new map source
	//
	GoogleMapSource::GoogleMapSource( GoogleMapKind mapKind ) :
		mMapKind( mapKind )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	GoogleMapSource::~GoogleMapSource( )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	inline static double exp( double x ) 
	//-------------------------------------------------------------------------
	{
		return pow( M_E, x ); 
	}

	//-------------------------------------------------------------------------
	inline static double Resolution( MagnificationType magnification )
	//-------------------------------------------------------------------------
	{
		return InitialResolution / pow(2.0, (double)magnification);
	}

	//-------------------------------------------------------------------------
	//
	// Converts given lat/lon in WGS84 Datum to XY in Spherical Mercator EPSG:900913
	//
	inline static void LonLatToMeters( const double lon, const double lat, double& meterX, double& meterY )
	//-------------------------------------------------------------------------
	{
		meterX = lon * OriginShift / 180.0;
		meterY = log( tan( ( 90.0 + lat ) * PI / 360.0 ) ) * OriginShift / PI;
	}

	//-------------------------------------------------------------------------
	//
	// Converts XY point from Spherical Mercator EPSG:900913 to lat/lon in WGS84 Datum
	//
	inline static void MetersToLonLat( const double metersX, const double metersY, double& lon, double& lat )
	//-------------------------------------------------------------------------
	{
		lon = metersX / OriginShift * 180.0;
		lat = 360.0 / PI * atan( exp( metersY * PI / OriginShift ) ) - 90.0;
	}

	//-------------------------------------------------------------------------
	//
	// Converts pixel coordinates in given zoom level of pyramid to EPSG:900913
	//
	inline static void PixelsToMeters( const double pixelX, const double pixelY, const MagnificationType magnification, double& meterX, double& meterY )
	//-------------------------------------------------------------------------
	{
		double res = Resolution( magnification );
		meterX = pixelX * res - OriginShift;
		meterY = pixelY * res - OriginShift;
	}

	//-------------------------------------------------------------------------
	//
	// Converts EPSG:900913 to pyramid pixel coordinates in given zoom level
	//
	inline static void MetersToPixels( const double meterX, const double meterY, const MagnificationType magnification, double& pixelX, double& pixelY )
	//-------------------------------------------------------------------------
	{
		double res = Resolution( magnification );
		pixelX = ( meterX + OriginShift ) / res;
		pixelY = ( meterY + OriginShift ) / res;
	}

	//-------------------------------------------------------------------------
	//
	// Returns a tile covering region in given pixel coordinates
	//
	inline static void PixelsToTile( const int tileSize, const double pixelX, const double pixelY, int& tileX, int& tileY )
	//-------------------------------------------------------------------------
	{
		tileX = (int)( ceil( pixelX / (double)tileSize ) - 1 );
		tileY = (int)( ceil( pixelY / (double)tileSize ) - 1 );
	}

	//-------------------------------------------------------------------------
	//
	// Returns tile for given mercator coordinates
	//
	inline static void MetersToTile( const int tileSize, const double meterX, const double meterY, const int magnification, int& tileX, int& tileY )
	//-------------------------------------------------------------------------
	{
		double pixelX, pixelY;
		MetersToPixels( meterX, meterY, magnification, pixelX, pixelY );
		PixelsToTile( tileSize, pixelX, pixelY, tileX, tileY );
	}

	//-------------------------------------------------------------------------
	//
	// Returns bounds of the given tile in EPSG:900913 coordinates
	//
	inline static void TileCenter( const int tileSize, const int tileX, const int tileY, const MagnificationType magnification, double& meterX, double& meterY )
	//-------------------------------------------------------------------------
	{
		PixelsToMeters( (tileX + 0.5f ) * tileSize, ( tileY + 0.5f ) * tileSize, magnification, meterX, meterY );
	}

	//-------------------------------------------------------------------------
	MapTileCoordinate GoogleMapSource::lonLatToTile( LonLat lonlat, MagnificationType magnification )
	//-------------------------------------------------------------------------
	{
		double meterX, meterY;
		int tileX, tileY;
		LonLatToMeters( lonlat.lon, lonlat.lat, meterX, meterY );
		MetersToTile( getTileSize( ), meterX, meterY, magnification, tileX, tileY );
		return MapTileCoordinate( tileX, tileY, magnification );
	}

	//-------------------------------------------------------------------------
	PixelCoordinate GoogleMapSource::lonLatToPixel( LonLat lonlat, MagnificationType magnification )
	//-------------------------------------------------------------------------
	{
		double meterX, meterY;
		double pixelX, pixelY;
		LonLatToMeters( lonlat.lon, lonlat.lat, meterX, meterY );
		MetersToPixels( meterX, meterY, magnification, pixelX, pixelY );
		return PixelCoordinate( magnification, (int)pixelX, (int)pixelY );
	}

	//-------------------------------------------------------------------------
	//
	// Convert tile center plus pixel offset to lat/lon. Offset increases right and down
	//
	LonLat GoogleMapSource::tileCenterToLonLat( const int tileSize, const MapTileCoordinate& tile, const double offsetX, const double offsetY )
	//-------------------------------------------------------------------------
	{
		double meterX, meterY;
		TileCenter( getTileSize( ), tile.getX( ), tile.getY( ), tile.getMagnification( ), meterX, meterY );
		
		double pixelX, pixelY;
		MetersToPixels( meterX, meterY, tile.getMagnification( ), pixelX, pixelY );
		pixelX += offsetX;
		pixelY -= offsetY; // google maps y coords increase up
		PixelsToMeters( pixelX, pixelY, tile.getMagnification( ), meterX, meterY );
		
		double lon, lat;
		MetersToLonLat( meterX, meterY, lon, lat );
		
		return LonLat( lon, lat );
	}

	//-------------------------------------------------------------------------
	//
	// Requests a map image from Google Static Maps.
	// URL example:
	// OLD: http://maps.google.com/staticmap?center=59.4,18.4&zoom=13&size=640x640&key=MAPS_API_KEY
	// http://maps.google.com/maps/api/staticmap?parameterscenter=59.4,18.4&zoom=13&size=640x640&key=MAPS_API_KEY
	// http://maps.google.com/maps/api/staticmap?center=59.4,18.4&zoom=13&size=640x640&sensor=false&maptype=streetmap&format=png8&key=ABQIAAAAzr2EBOXUKnm_jVnk0OJI7xSsTL4WIgxhMZ0ZK_kHjwHeQuOD4xQJpBVbSrqNn69S6DOTv203MQ5ufA
	//
	void GoogleMapSource::getTileUrl( char* buffer, MapTileCoordinate tileXY )
	//-------------------------------------------------------------------------
	{
		const int bottomCrop = 30;
		int tileSize = getTileSize( );
		LonLat centerpoint = this->tileCenterToLonLat( tileSize, tileXY, 0, bottomCrop/2 );
		const char* maptype = "";
		switch( mMapKind )
		{
		case GoogleMapKind_StreetMap: maptype = "roadmap"; break;
		case GoogleMapKind_Aerial: maptype = "satellite"; break;
		case GoogleMapKind_Hybrid: maptype = "hybrid"; break;
		default: BIG_PHAT_ERROR;
		}
		sprintf( buffer,
				"http://maps.google.com/maps/api/staticmap?center=%f,%f&zoom=%d&size=%dx%d&sensor=false&maptype=%s&format=png8&key=%s",
				centerpoint.lat,
				centerpoint.lon,
				tileXY.getMagnification( ),
				tileSize,
				tileSize + bottomCrop, // this will cut out the source notice at the bottom of each tile
				maptype,
				ApiKey
				);
	}
}
