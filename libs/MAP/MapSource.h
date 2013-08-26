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
* \file MapSource.h
* \brief Abstract base class for provider of map tiles
* \author Lars-Åke Vinberg
*/

#ifndef MAPSOURCE_H_
#define MAPSOURCE_H_

#include <maapi.h>
#include <MAUtil/Downloader.h>

#include "MapTile.h"
#include "Queue.h"
#include "Broadcaster.h"
#include "MapTileCoordinate.h"



namespace MAP
{
	using namespace MAUtil;
	
	//class LonLat;
	class MapTile;
	class MapSourceImageDownloader;
	class MapSourceQueue;
	class MapSource;

	//=========================================================================
	/**
	 * \brief Listener for MapSource client to implement.
	 */
	class IMapSourceListener
	//=========================================================================
	{
	public:
		virtual void	 tileReceived( MapSource* sender, MapTile* tile ) = 0;
		virtual void	 downloadCancelled( MapSource* sender ) = 0;
		virtual void	 error( MapSource* source, int code ) = 0;
		virtual void	 jobComplete( MapSource* source ) = 0;
	};

	//=========================================================================
	/**
	 * \brief Base class for map sources.
	 * 
	 * A map source is a provider of map tiles.
	 * Behavior is modeled after OpenStreetMap tile server.
	 */
	class MapSource : public DownloadListener
	//=========================================================================
	{
	public:
									MapSource( );
		virtual						~MapSource( );

		//============================================================
		//
		// virtual functions for client to override
		//
		//============================================================
		/**
		 * Returns tile size
		 */
		virtual  int                getTileSize( ) const = 0;
		/**
		 * Returns minimum magnification
		 */
		virtual int					getMagnificationMin( ) const = 0;
		/**
		 * Returns maximum magnification
		 */
		virtual int					getMagnificationMax( ) const = 0;
		/**
		 * Returns URL for retrieving a tile
		 */
		virtual void				getTileUrl( char* buffer, MapTileCoordinate tileXY ) = 0;
		/**
		 * Converts LonLat to a pixel coordinate, in a global pixel grid
		 */
		virtual PixelCoordinate		lonLatToPixel( LonLat lonlat, MagnificationType magnification ) = 0;
		/**
		 * Converts LonLat to tile coordinates for a tile that contains the lonlat point.
		 */
		virtual MapTileCoordinate	lonLatToTile( LonLat lonlat, MagnificationType magnification ) = 0;
		/**
		 * Convert tile center plus pixel offset to WGS84 lat/lon.
		 */
		virtual LonLat				tileCenterToLonLat( const int tileSize, const MapTileCoordinate& tile, const double offsetX, const double offsetY ) = 0;
		/**
		 * Returns all tiles required to cover specified rectangle around centerpoint.
		 */
		void						requestTile( IMapSourceListener* listener, const MapTileCoordinate tileXY );
		/*
		 * Adds a job complete entry to queue, so client can be notified when a sequence of tiles has been delivered.
		 */
		void						requestJobComplete( IMapSourceListener* listener );
		/**
		 * Clears any queued requests
		 */
		void						clearQueue( );
		//
		// Number of tiles received
		//
		int							getTileCount( ) { return mTileCount; }
		//
		// DownloadListener overrides
		//
		void						finishedDownloading( Downloader* downloader, MAHandle data );
		void						downloadCancelled(Downloader* downloader);
		void						error(Downloader* downloader, int code);

	private:
		//
		// returns position of empty downloader slot, or -1 if all downloaders are busy.
		//
		int							findUnusedSlot( MapSourceImageDownloader* protectedDownloader );
		void						dequeueIfIdleSlot( MapSourceImageDownloader* protectedDownloader );
		//
		//
		//
		void						dequeueNextJob( MapSourceImageDownloader* downloader );
		//
		// Is job already in queue?
		//
		bool						isInQueue( MapTileCoordinate tileXY );
		bool						isInDownloaders( MapTileCoordinate tileXY );
		//
		//
		//
		void						removeDownloader( MapSourceImageDownloader* downloader );
		//
		// Event notifiers
		//
		void						onJobComplete( IMapSourceListener* listener );
		void						onTileReceived( IMapSourceListener* listener, MapTile* tile );
		void						onDownloadCancelled( IMapSourceListener* listener );
		void						onError( IMapSourceListener* listener, int code );

		MapSourceQueue*				mQueue;
		MapSourceImageDownloader**	mDownloaders;
		int							mTileCount;
	};
}

#endif // MAPSOURCE_H_
