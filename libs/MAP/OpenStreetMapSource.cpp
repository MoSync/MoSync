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

#include "OpenStreetMapSource.h"
#include "MapTileCoordinate.h"

namespace MAP
{
	//
	//	Internal coord conversions
	//
	inline static double exp( double x )				
	{
		return pow( M_E, x ); 
	}
	
	inline static int long2tile( double lon, int z )	
	{
		return (int)( floor( (lon + 180.0) / 360.0 * pow(2.0, z) ) ); 
	}

	inline static int lat2tile( double lat, int z )		
	{
		return (int)( floor( (1.0 - log( tan( lat * M_PI / 180.0 ) + 1.0 / cos( lat * M_PI/180.0 ) ) / M_PI ) / 2.0 * pow( 2.0, z ) ) ); 
	}
	
	inline static double tile2long( int x, int z )		
	{ 
		return x / pow( 2.0, z ) * 360.0 - 180; 
	}
	
	inline static double tile2lat( int y, int z )		
	{ 
		double n = M_PI - 2.0 * M_PI * y / pow( 2.0, z ); 
		return 180.0 / M_PI * atan( 0.5 * ( exp( n ) - exp( -n ) ) ); 
	}
	
	//-------------------------------------------------------------------------
	OpenStreetMapSource::OpenStreetMapSource( ) 
	//-------------------------------------------------------------------------
		: MapSource( )
	{
	}
		
	//-------------------------------------------------------------------------
	OpenStreetMapSource::~OpenStreetMapSource( )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	MapTileCoordinate OpenStreetMapSource::lonLatToTile( LonLat lonlat, int magnification )
	//-------------------------------------------------------------------------
	{
		return MapTileCoordinate( long2tile( lonlat.lon, magnification ), lat2tile( lonlat.lat, magnification ), magnification );
	}

	//-------------------------------------------------------------------------
	inline static double Resolution( int magnification)
	//-------------------------------------------------------------------------
	{
		return InitialResolution / ( 1 << magnification );
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
		lat = 360.0 / PI * atan( exp( -metersY * PI / OriginShift ) ) - 90.0;
	}

	//-------------------------------------------------------------------------
	//
	// Converts pixel coordinates in given zoom level of pyramid to EPSG:900913
	//
	inline static void PixelsToMeters( const double pixelX, const double pixelY, const int magnification, double& meterX, double& meterY )
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
	inline static void MetersToPixels( const double meterX, const double meterY, const int magnification, double& pixelX, double& pixelY )
	//-------------------------------------------------------------------------
	{
		double res = Resolution( magnification );
		pixelX = ( meterX + OriginShift ) / res;
		pixelY = ( meterY + OriginShift ) / res;
	}

	//-------------------------------------------------------------------------
	//
	// Returns center of the given tile in meters
	//
	inline static void TileCenterMeters( const int tileSize, const int tileX, const int tileY, const int magnification, double& meterX, double& meterY )
	//-------------------------------------------------------------------------
	{
		PixelsToMeters( (tileX + 0.5f ) * tileSize, ( tileY + 0.5f ) * tileSize, magnification, meterX, meterY );
	}

	//-------------------------------------------------------------------------
	//
	// Returns center of the given tile in pixels
	//
	inline static void TileCenterPixels( const int tileSize, const int tileX, const int tileY, double& pixelX, double& pixelY )
	//-------------------------------------------------------------------------
	{
		pixelX = (tileX + 0.5f ) * tileSize;
		pixelY = ( tileY + 0.5f ) * tileSize;
	}

	//-------------------------------------------------------------------------
	PixelCoordinate OpenStreetMapSource::lonLatToPixel( LonLat lonlat, int magnification )
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
	LonLat OpenStreetMapSource::tileCenterToLonLat( const int tileSize, const MapTileCoordinate& tile, const double offsetX, const double offsetY )
	//-------------------------------------------------------------------------
	{
		double meterX, meterY;
		double pixelX, pixelY;
		
		TileCenterPixels( tileSize, tile.getX( ), tile.getY( ), pixelX, pixelY );
		pixelX += offsetX;
		pixelY += offsetY;
		PixelsToMeters( pixelX, pixelY, tile.getMagnification( ), meterX, meterY );
		double lon, lat;
		MetersToLonLat( meterX, meterY, lon, lat );
		return LonLat( lon, lat );
	}

	//-------------------------------------------------------------------------
	//
	// Requests a map image from OpenStreetMaps tile API
	// URL example:
	// http://tile.openstreetmap.org/7/63/42.png
	// where params are
	//	- magnification
	//	- tile x
	//	- tile y
	// according to coordinate system specified at
	// http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames
	//
	// Alternate map (does not look good as of 20090526)
	// http://tah.openstreetmap.org/Tiles/tile/12/2047/1362.png
	//
	void OpenStreetMapSource::getTileUrl( char* buffer, MapTileCoordinate tileXY )
	//-------------------------------------------------------------------------
	{
		sprintf( buffer, "http://tile.openstreetmap.org/%d/%d/%d.png", tileXY.getMagnification( ), tileXY.getX( ), tileXY.getY( ) );
	}
}
