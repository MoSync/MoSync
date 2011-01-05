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
* \file MapTile.h
* \brief Single tile from a map source
* \author Lars-Åke Vinberg
*/

#ifndef MAPTILE_H_
#define MAPTILE_H_

#include "DateTime.h"
#include "LonLat.h"

using namespace MAPUtil;

namespace MAP
{
	class MapSource;

	//=========================================================================
	/**
	 * \brief Single tile from a map source
	 *
	 * Tiles use a numbering scheme like OpenStreetMap or Google Maps.
	 * http://www.maptiler.org/google-maps-coordinate-system-projection-epsg-900913-3785/
	 */
	class MapTile
	//=========================================================================
	{
	public:
		/**
		 * Creates a map tile.
		 */
		MapTile( MapSource* source, const int gridX, const int gridY, const int magnification, const LonLat center, MAHandle image ) :
			mSource( source ),
			mGridX( gridX ),
			mGridY( gridY ),
			mMagnification( magnification ),
			mCenter( center ),
			mImage( image ),
			mLastAccessTime( DateTime::minValue( ) )
		{
		}
		/**
		 * Destroys a map tile.
		 */
		virtual ~MapTile( )
		{
			maDestroyObject( mImage );
		}
		/**
		 * Sets last access timestamp to current time.
		 */
		void stamp( ) 
		{ 
			mLastAccessTime = DateTime::now( ); 
		}
		/**
		 * Returns the source for this tile.
		 */
		MapSource* getMapSource( ) const
		{
			return mSource;
		}
		/**
		 * Returns X coordinate of tile in tile grid.
		 */
		int getGridX( ) const					
		{ 
			return mGridX; 
		}
		/**
		 * Returns Y coordinate of tile in tile grid.
		 */
		int getGridY( ) const					
		{ 
			return mGridY; 
		}
		/**
		 * Returns zoom level of tile.
		 */
		int getMagnification( ) const			
		{ 
			return mMagnification; 
		}
		/**
		 * Returns center of tile in longitude/latitude.
		 */
		LonLat getCenter( ) const				
		{ 
			return mCenter; 
		}
		/**
		 * Returns the map image of the tile.
		 */
		MAHandle getImage( ) const				
		{ 
			return mImage; 
		}
		/**
		 * Returns last access time of the tile.
		 */
		DateTime getLastAccessTime( ) const		
		{ 
			return mLastAccessTime; 
		}

	private:
		MapSource* mSource;
		int mGridX;
		int mGridY;
		int mMagnification;
		LonLat mCenter;
		MAHandle mImage;
		DateTime mLastAccessTime;
	};
}
#endif // MAPTILE_H_
