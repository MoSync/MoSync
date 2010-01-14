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

#ifndef MAPSOURCE_H_
#define MAPSOURCE_H_

#include <maapi.h>
#include <MAUtil/downloader.h>

#include "MapTile.h"
#include "Queue.h"

using namespace MAUtil;

namespace MAP
{
	class LonLat;
	class MapTile;
	class MapSourceImageDownloader;
	class MapSourceQueue;
	class MapSource;

	//=========================================================================
	//
	// Client data must subclass this
	//
	class MapSourceClientData
	//=========================================================================
	{
	public:
		virtual			~MapSourceClientData( ) { };
	};

	//=========================================================================
	class IMapSourceListener
	//=========================================================================
	{
	public:
		virtual void	tileReceived( MapSource* sender, MapTile* tile, MapSourceClientData* clientData ) = 0;
		virtual void	downloadCancelled( MapSource* sender ) = 0;
		virtual void	error( MapSource* source, int code ) = 0;
	};

	//=========================================================================
	class MapSource : public DownloadListener
	//=========================================================================
	{
	private:
		static const int			QueueSize = 100;
		static const int			Downloaders = 5; // four in parallel plus one slacking
	public:
									MapSource( );
		virtual						~MapSource( );

		void						setApiKey( const char* key );
		const char*					getApiKey( ) const;

		//============================================================
		//
		// virtual functions for client to override
		//
		//============================================================
		//
		// Returns source kind
		//
		virtual MapSourceKind		getSourceKind( ) const = 0;
		//
		// Returns tile size
		//
		virtual  MAExtent			getTileSize( ) const = 0;
		//
		// Returns minimum magnification
		//
		virtual int					getMagnificationMin( ) const = 0;
		//
		// Returns maximum magnification
		//
		virtual int					getMagnificationMax( ) const = 0;
		//
		// Returns URL for retrieving a tile
		//
		virtual void				getTileUrl( char* buffer, MapTileCoordinate tileXY ) = 0;
		//
		// Converts LonLat to a pixel coordinate, in a global pixel grid
		//
		virtual PixelCoordinate		lonLatToPixel( LonLat lonlat, int magnification ) = 0;
		//
		// Converts LonLat to tile coordinates for a tile that contains the lonlat point.
		//
		virtual MapTileCoordinate	lonLatToTile( LonLat lonlat, int magnification ) = 0;
		//
		// Convert tile center plus pixel offset to WGS84 lat/lon.
		//
		virtual LonLat				tileCenterToLonLat( const int tileSize, const MapTileCoordinate& tile, const double offsetX, const double offsetY ) = 0;
		//
		// Returns all tiles required to cover specified rectangle around centerpoint.
		//
		void						requestTile( const MapTileCoordinate tileXY, IMapSourceListener* listener, MapSourceClientData* clientData );
		//
		// Clears any queued requests
		//
		void						clearQueue( );
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
		//
		//
		//
		void						removeDownloader( MapSourceImageDownloader* downloader );

		MapSourceQueue*				m_queue;
		MapSourceImageDownloader*	m_downloaders[Downloaders];

	};
}

#endif // MAPSOURCE_H_
