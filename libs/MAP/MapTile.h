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
* \file MapTile.h
* \brief Single tile from a map source
* \author Lars-Åke Vinberg
*/

#ifndef MAPTILE_H_
#define MAPTILE_H_

#include "DateTime.h"
#include "LonLat.h"
#include "DebugPrintf.h"
#include "TraceScope.h"



namespace MAP
{

	using namespace MAPUtil;

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
		MapTile( MapSource* source, const int gridX, const int gridY, const int magnification, const LonLat center, MAHandle image
				#ifdef StoreCompressedTilesInCache
				, int contentLength
				#endif
			) :
			mSource( source ),
			mGridX( gridX ),
			mGridY( gridY ),
			mMagnification( magnification ),
			mCenter( center ),
			mImage( image ),
			mLastAccessTime( DateTime::minValue( ) ),
			mCreationTime( maGetMilliSecondCount() )
			#ifdef StoreCompressedTilesInCache
			, mContentLength( contentLength )
			#endif
		{
		}
		/**
		 * Destroys a map tile.
		 */
		virtual ~MapTile( )
		{
			maDestroyPlaceholder(mImage);
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

		/**
		 * Returns milliseconds since the tile was created.
		 */
		int getMilliSecondsSinceCreated( ) const
		{
			return maGetMilliSecondCount() - mCreationTime;
		}

		#ifdef StoreCompressedTilesInCache

		int getContentLength( ) const
		{
			return mContentLength;
		}

		#endif

	private:
		MapSource* mSource;
		int mGridX;
		int mGridY;
		int mMagnification;
		LonLat mCenter;
		MAHandle mImage;
		DateTime mLastAccessTime;
		int mCreationTime;
		int mContentLength;
	};
}
#endif // MAPTILE_H_
