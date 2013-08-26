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
* \file MapCache.h
* \brief Cache of tiles provided by MapSource
* \author Lars-Åke Vinberg
*/

#ifndef MAPCACHE_H_
#define MAPCACHE_H_

#include <MAUtil/HashMap.h>
#include "DateTime.h"

#include "MapSource.h"

namespace MAP
{
	class MapTile;
	class MapCache;
	class MapSource;

	//=========================================================================
	/**
	 * \brief Listener class, for listening client to implement.
	 */
	class IMapCacheListener
	//=========================================================================
	{
	public:
		/**
		 * Called when a requested tile has been received into cache from map source.
		 */
		virtual void tileReceived( MapCache* sender, MapTile* tile, bool foundInCache ) = 0;
		/**
		 * Called when a tile job is completed.
		 */
		virtual void jobComplete( MapCache* sender ) = 0;
		/**
		 * Called when an error occurred, likely when MapSource failed to download tile.
		 */
		virtual void error( MapCache* sender, int code ) = 0;
	};

	class MapTileKey;

	//=========================================================================
	/**
	 * \brief Manages map caches for clients to access.
	 * Implemented as singleton.
	 */
	class MapCache : IMapSourceListener,
		public Broadcaster<IMapCacheListener>
	//=========================================================================
	{
	private:
		MapCache( );

	public:
		/**
		 * Returns the MapCache singleton.
		 */
		static MapCache* get( );
		/**
		 * Shuts down map cache properly. 
		 * Call at application shutdown to free all resources.
		 */
		static void shutdown( );

		virtual ~MapCache( );
		/**
		 * Requests tiles to cover specified rectangle, in pixels,
		 * around a centerpoint.
		 */
		void requestTiles( MapSource* source, const LonLat centerpoint, const MagnificationType magnification, const int pixelWidth, const int pixelHeight, const double directionX, const double directionY );
		/**
		 * Frees all tiles in cache.
		 */
		void clear( );
		//
		// IMapSourceListener implementation
		//
		void tileReceived( MapSource* sender, MapTile* tile );
		void downloadCancelled( MapSource* sender );
		void error( MapSource* source, int code );
		void jobComplete( MapSource* source );
		//
		// Capacity property
		//
		int getCapacity( ) const;
		void setCapacity( int capacity );
		int size( );

	private:
		static MapCache* sSingleton;
		/**
		 * returns location of least recently used tile
		 */
		MapTileKey findLRU( );

		void onTileReceived( MapTile* tile, bool foundInCache );
		void onJobComplete( );
		void onError( int code );

		HashMap<MapTileKey, MapTile*> mList;
		int mHits;
		int mMisses;
		int mCapacity;
	};
}

#endif // MAPCACHE_H_

